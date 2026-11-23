/* 
 * Name: Nicholas Orndoff
 * File: krylov_tools.hpp
 * 
 * Summary: 
 * This header file declares the Object-Oriented architecture for my image processing application.
 * It defines the custom Vector and Image classes for encapsulating pixel data. It also defines 
 * an abstract base class (ImageOperator) and a hierarchy of 12 derived classes (ConvolutionOperator, 
 * GaussianBlurOperator, InvertOperator, etc.) to represent various image filters using polymorphism. 
 * Finally, it includes UI classes (Menu, ImageEditorApp, AsciiPreview) and IO classes (ImageLoader, 
 * ImageSaver) to manage user interactions and file handling.
 */

#ifndef KRYLOV_TOOLS_HPP
#define KRYLOV_TOOLS_HPP

#include <algorithm>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// 3rd party image libraries
#include "stb_image.h"
#include "stb_image_write.h"

// Vector
class Vector {
    std::vector<double> d_;
public:
    Vector() = default;
    explicit Vector(size_t n, double v = 0.0) : d_(n, v) {}
    Vector(std::initializer_list<double> l) : d_(l) {}

    size_t size() const { return d_.size(); }
    bool empty() const { return d_.empty(); }
    double& operator[](size_t i) { return d_[i]; }
    double operator[](size_t i) const { return d_[i]; }
    double* data() { return d_.data(); }
    const double* data() const { return d_.data(); }
    
    double* begin() { return d_.data(); }
    double* end() { return d_.data() + d_.size(); }
    const double* begin() const { return d_.data(); }
    const double* end() const { return d_.data() + d_.size(); }

    double dot(const Vector& o) const;
    double norm() const;
};

// Rng 
class Rng {
    std::mt19937_64 g_;
public:
    explicit Rng(unsigned long long seed) : g_(seed) {}
    double uniform();
    double normal();
};

// Image 
class Image {
    int m_ = 0, n_ = 0, ch_ = 1;
    Vector px_;
public:
    Image() = default;
    Image(int m, int n, int ch = 1) : m_(m), n_(n), ch_(ch), px_((size_t)m * n * ch, 0.0) {}

    int rows() const { return m_; }
    int cols() const { return n_; }
    int channels() const { return ch_; }
    int pixel_count() const { return m_ * n_; }
    size_t total_values() const { return px_.size(); }

    const Vector& pixels() const { return px_; }
    Vector& pixels() { return px_; }

    double at(int r, int c, int ch = 0) const;
    void set(int r, int c, double v, int ch = 0);
    Image clone() const { return *this; }
    
    void clamp(double lo, double hi);
    double min_value() const;
    double max_value() const;
    double mean_value() const;
};

//  Operators 
class ImageOperator {
public:
    virtual ~ImageOperator() = default;
    virtual Image apply(const Image& in) const = 0;
    virtual std::string name() const = 0;
};

class ConvolutionOperator : public ImageOperator {
protected:
    int k_, radius_;
    Vector kernel_;
    ConvolutionOperator(int k, Vector kernel) : k_(k), radius_(k / 2), kernel_(std::move(kernel)) {}
public:
    Image apply(const Image& in) const override;
};

class GaussianBlurOperator : public ConvolutionOperator {
    double sigma_;
public:
    GaussianBlurOperator(int k, double sigma);
    std::string name() const override;
private:
    static Vector make_kernel(int k, double sigma);
};

class BoxBlurOperator : public ConvolutionOperator {
public:
    explicit BoxBlurOperator(int k) : ConvolutionOperator(k, Vector((size_t)k * k, 1.0 / (k * k))) {}
    std::string name() const override;
};

class SharpenOperator : public ConvolutionOperator {
public:
    SharpenOperator() : ConvolutionOperator(3, {0, -1, 0, -1, 5, -1, 0, -1, 0}) {}
    std::string name() const override;
};

class MedianFilterOperator : public ImageOperator {
    int k_, radius_;
public:
    explicit MedianFilterOperator(int k) : k_(k), radius_(k / 2) {}
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class SobelOperator : public ImageOperator {
public:
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class InvertOperator : public ImageOperator {
public:
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class GammaOperator : public ImageOperator {
    double g_;
public:
    explicit GammaOperator(double g) : g_(g) {}
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class ThresholdOperator : public ImageOperator {
    double t_;
public:
    explicit ThresholdOperator(double t) : t_(t) {}
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class GrayscaleOperator : public ImageOperator {
public:
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class GaussianNoiseOperator : public ImageOperator {
    double sigma_;
    mutable unsigned long long seed_;
public:
    explicit GaussianNoiseOperator(double s, unsigned long long seed = 42) : sigma_(s), seed_(seed) {}
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class SaltPepperOperator : public ImageOperator {
    double p_;
    mutable unsigned long long seed_;
public:
    explicit SaltPepperOperator(double p, unsigned long long seed = 42) : p_(p), seed_(seed) {}
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class FlipHorizontalOperator : public ImageOperator {
public:
    Image apply(const Image& in) const override;
    std::string name() const override;
};

class FlipVerticalOperator : public ImageOperator {
public:
    Image apply(const Image& in) const override;
    std::string name() const override;
};

//  IO and App 
class ImageLoader {
public:
    static Image load(const std::string& filename);
};

class ImageSaver {
public:
    static void save(const std::string& filename, const Image& img);
};

class AsciiPreview {
    int width_;
public:
    explicit AsciiPreview(int w = 60) : width_(w) {}
    std::string render(const Image& img) const;
};

class Menu {
    std::string title_;
    std::vector<std::pair<std::string, std::function<void()>>> items_;
public:
    explicit Menu(std::string title) : title_(std::move(title)) {}
    Menu& add(std::string label, std::function<void()> action);
    void run() const;
};

class ImageEditorApp {
    Image original_, current_;
    std::string filename_;
    std::vector<std::pair<std::string, Image>> history_;
public:
    void run();
private:
    bool prompt_load();
    static int prompt_int(const std::string& p, int lo, int hi);
    static double prompt_double(const std::string& p, double lo, double hi);
    static std::string prompt_line(const std::string& p);
    static int odd_kernel(const std::string& p, int lo, int hi);
    void apply(std::unique_ptr<ImageOperator> op);
    Menu main_menu();
    Menu filters_menu();
    Menu point_menu();
    Menu noise_menu();
    void info() const;
    void preview() const;
    void save_image();
    void show_history() const;
    void undo();
};

#endif 