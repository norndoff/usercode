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

#endif