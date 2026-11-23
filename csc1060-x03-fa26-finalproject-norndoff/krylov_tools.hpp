#ifndef KRYLOV_TOOLS_HPP
#define KRYLOV_TOOLS_HPP

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

inline std::string fmt_num(double v, int prec = 3) {
    std::ostringstream s;
    s.precision(prec);
    s << v;
    return s.str();
}

inline int clampi(int v, int lo, int hi) { return std::max(lo, std::min(hi, v)); }

class Vector {
    std::vector<double> d_;

    public: 
        Vector() = default;
        explicit Vector(size_t n, double v = 0.0) : d_(n, v) {}
        
        Vector(std::initializer_list<double> l) : d_(l) {}

        size_t size() const { return d_.size(); }
        bool empty() const { return d_.empty(); }
        double& operator[](size_t i) { return d_[i]; }
        double operator[](size_t i) const {return d_[i]; }
        double* data() { return d_.data(); }
        const double* data() const { return d_.data(); }
        double* begin() { return d_.data(); }
        double* end() { return d_.data() + d_.size(); }
        const double* begin() const { return d_.data(); }
        const double* end() const { return d_.data() + d_.size(); }

        double dot(const Vector& o) const {
            if (o.d_.size() != d_.size())
                throw std::runtime_error("Vector::dot size mismatch");
            double s = 0;
            for (size_t i = 0; i < d_.size(); ++i) s+= d_[i] *o.d_[i];
            return s;
        }
        double norm() const { return std::sqrt(dot(*this)); }
};

// every computer gets same noise for same seed.
class Rng {
    std::mt19937_64 g_;

    public:
        explicit Rng(unsigned long long seed) : g_(seed) {}
        double uniform() { return (double(g_() >> 11) + 0.5) / 90071992547400992.0; }
        double normal() {
            double u1 = std::max(uniform(), 1e-300), u2 = uniform();
            return std::sqrt(-2.0 * std::log(u1)) * std::cos(6.283185307179586 * u2);
        }
};

class Image {
    int m_ = 0, n_ = 0, ch_ = 1;
    Vector px_;

    public:
        Image() = default;
        Image(int m, int n, int ch = 1) 
            : m_(m), n_(n), ch_(ch), px_((size_t)m * n * ch, 0.0) {}
        
        int rows() const { return m_; }
        int cols() const { return n_; }
        int channels() const { return ch_; }
        int pixel_count() const { return px_.size(); }

        const Vector& pixels() const { return px_; }
        Vector& pixels() { return px_; }

        double at(int r, int c, int ch = 0) const {
            return px_[(size_t)ch * m_ * n_ + (size_t)r * n_ + c];
        }

        void set(int r, int c, double v, int ch = 0) {
            px_[(size_t)ch * m_ * n_ + (size_t)r * n_ + c] = v;
        }
        
        Image clone() const { return *this; }

        void clamp(double lo, double hi) {
            for (auto& v : px_) v = std::min(hi, std::max(lo, v));
        }
        double min_value() const {
            if (px_.empty()) return 0;
            double m = px_[0];
            for (size_t i = 1; i < px_.size(); ++i) 
            m = std::min(m, px_[i]);
            return m;
        }
        double max_value() const {
            if (px_.empty()) return 0;
            double m = px_[0];
            for (size_t i = 1; i < px_.size(); ++i)
            m = std::max(m, px_[i]);
            return m;
        }
        double mean_value() const {
            if (px_.empty()) return 0;
            double s = 0;
            for (size_t i = 1; i < px_.size(); ++i)
            s += px_[i];
            return s / px_.size();
        }
};

// Abstract base
class ImageOperator {
    public:
    virtual ~ImageOperator() = default;
    virtual Image apply(const Image& in) const = 0;
    virtual std::string name() const = 0;
    virtual std::unique_ptr<ImageOperator> clone() const = 0;
};

// Template Method
class ConvolutionOperator : public ImageOperator {
    protected:
        int k_, radius_;
        Vector kernel_;
     ConvolutionOperator(int k, Vector kernel) : k_(k), radius_(k / 2), kernel_(std::move(kernel)) {}

    public: 
        Image apply(const Image& in) const override{
            Image out = in.clone();
        for (int ch = 0; ch < in.channels(); ++ch)
            for (int i = 0; i < in.rows(); ++i)
                for (int j = 0; j < in.cols(); ++j) {
                    double s = 0;
                    for (int a = 0; a < k_; ++a) {
                        int sr = i + radius_ - a;
                        if (sr < 0 || sr >= in.rows()) continue;
                        for (int b = 0; b < k_; ++b) {
                            double w = kernel_[(size_t)a * k_ + b];
                            if (w == 0) continue;
                            int sc = j + radius_ - b;
                            if (sc < 0 || sc >= in.cols()) continue;
                            s += w * in.at(sr, sc, ch);
                        }
                    }
                    out.set(i, j, s, ch);
                }
            return out;
        }
};

// Concrete Filters
class GaussianBlurOperator : public ConvolutionOperator {
    double sigma_;

    public: 
        GaussianBlurOperator(int k, double sigma) : ConvolutionOperator(k, make_kernel(k, sigma)), sigma_(sigma) {}

        std::string name() const override {
            return "Gaussian blur (k=" + std::to_string(k_) + ", sigma=" + fmt_num(sigma_) + ")";
        }
        std::unique_ptr<ImageOperator> clone() const override {
            return std::make_unique<GaussianBlurOperator>(*this);
        }
    
    private:
        static Vector make_kernel(int k, double sigma) {
            Vector w((size_t)k * k);
        int r = k / 2;
        double s2 = sigma * sigma, tot = 0;
        for (int a = 0; a < k; ++a)
            for (int b = 0; b < k; ++b) {
                int dy = a - r, dx = b - r;
                double v = std::exp(-(dy*dy + dx*dx) / (2 * s2));
                w[(size_t)a * k + b] = v;
                tot += v;
            }
        for (size_t i = 0; i < w.size(); ++i) w[i] /= tot;
        return w;
        }
};

#endif