/*
Nicholas Orndoff
*/

#include "krylov_tools.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// ======================= Vector & Rng =======================
double Vector::dot(const Vector& o) const {
    if (o.size() != d_.size()) throw std::runtime_error("Vector::dot size mismatch");
    double s = 0;
    for (size_t i = 0; i < d_.size(); ++i) s += d_[i] * o.d_[i];
    return s;
}

double Vector::norm() const { return std::sqrt(dot(*this)); }

double Rng::uniform() { 
    return (double(g_() >> 11) + 0.5) / 9007199254740992.0; 
}

double Rng::normal() {
    double u1 = std::max(uniform(), 1e-300), u2 = uniform();
    return std::sqrt(-2.0 * std::log(u1)) * std::cos(6.283185307179586 * u2);
}

// ======================= Image =======================
double Image::at(int r, int c, int ch) const {
    return px_[(size_t)ch * m_ * n_ + (size_t)r * n_ + c];
}

void Image::set(int r, int c, double v, int ch) {
    px_[(size_t)ch * m_ * n_ + (size_t)r * n_ + c] = v;
}

void Image::clamp(double lo, double hi) {
    for (size_t i = 0; i < px_.size(); ++i) px_[i] = std::min(hi, std::max(lo, px_[i]));
}

double Image::min_value() const {
    if (px_.empty()) return 0;
    double m = px_[0];
    for (size_t i = 1; i < px_.size(); ++i) m = std::min(m, px_[i]);
    return m;
}

double Image::max_value() const {
    if (px_.empty()) return 0;
    double m = px_[0];
    for (size_t i = 1; i < px_.size(); ++i) m = std::max(m, px_[i]);
    return m;
}

double Image::mean_value() const {
    if (px_.empty()) return 0;
    double s = 0;
    for (size_t i = 0; i < px_.size(); ++i) s += px_[i];
    return s / px_.size();
}

// ======================= Operators =======================
Image ConvolutionOperator::apply(const Image& in) const {
    Image out = in.clone();
    for (int ch = 0; ch < in.channels(); ++ch) {
        for (int i = 0; i < in.rows(); ++i) {
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
        }
    }
    return out;
}

GaussianBlurOperator::GaussianBlurOperator(int k, double sigma) 
    : ConvolutionOperator(k, make_kernel(k, sigma)), sigma_(sigma) {}

Vector GaussianBlurOperator::make_kernel(int k, double sigma) {
    Vector w((size_t)k * k);
    int r = k / 2;
    double s2 = sigma * sigma, tot = 0;
    for (int a = 0; a < k; ++a) {
        for (int b = 0; b < k; ++b) {
            int dy = a - r, dx = b - r;
            double v = std::exp(-(dy * dy + dx * dx) / (2 * s2));
            w[(size_t)a * k + b] = v;
            tot += v;
        }
    }
    for (size_t i = 0; i < w.size(); ++i) w[i] /= tot;
    return w;
}

std::string GaussianBlurOperator::name() const { 
    return "Gaussian blur (k=" + std::to_string(k_) + ", sigma=" + std::to_string(sigma_).substr(0, 4) + ")"; 
}

std::string BoxBlurOperator::name() const { return "Box blur (k=" + std::to_string(k_) + ")"; }
std::string SharpenOperator::name() const { return "Sharpen (3x3)"; }

Image MedianFilterOperator::apply(const Image& in) const {
    Image out = in.clone();
    std::vector<double> win;
    win.reserve((size_t)k_ * k_);
    for (int ch = 0; ch < in.channels(); ++ch) {
        for (int i = 0; i < in.rows(); ++i) {
            for (int j = 0; j < in.cols(); ++j) {
                win.clear();
                for (int a = -radius_; a <= radius_; ++a) {
                    int sr = i + a;
                    if (sr < 0 || sr >= in.rows()) continue;
                    for (int b = -radius_; b <= radius_; ++b) {
                        int sc = j + b;
                        if (sc < 0 || sc >= in.cols()) continue;
                        win.push_back(in.at(sr, sc, ch));
                    }
                }
                auto mid = win.begin() + win.size() / 2;
                std::nth_element(win.begin(), mid, win.end());
                out.set(i, j, *mid, ch);
            }
        }
    }
    return out;
}

std::string MedianFilterOperator::name() const { return "Median filter (k=" + std::to_string(k_) + ")"; }

Image SobelOperator::apply(const Image& in) const {
    static const int kx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    static const int ky[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    Image out(in.rows(), in.cols(), 1);
    for (int i = 0; i < in.rows(); ++i) {
        for (int j = 0; j < in.cols(); ++j) {
            double gx = 0, gy = 0;
            for (int a = 0; a < 3; ++a) {
                int sr = i + a - 1;
                if (sr < 0 || sr >= in.rows()) continue;
                for (int b = 0; b < 3; ++b) {
                    int sc = j + b - 1;
                    if (sc < 0 || sc >= in.cols()) continue;
                    double v = 0;
                    for (int ch = 0; ch < in.channels(); ++ch) v += in.at(sr, sc, ch);
                    v /= in.channels();
                    gx += kx[a * 3 + b] * v;
                    gy += ky[a * 3 + b] * v;
                }
            }
            out.set(i, j, std::sqrt(gx * gx + gy * gy));
        }
    }
    return out;
}

std::string SobelOperator::name() const { return "Sobel edges"; }

Image InvertOperator::apply(const Image& in) const {
    Image out = in.clone();
    for (size_t i = 0; i < out.total_values(); ++i) out.pixels()[i] = 1.0 - out.pixels()[i];
    return out;
}
std::string InvertOperator::name() const { return "Invert"; }

Image GammaOperator::apply(const Image& in) const {
    Image out = in.clone();
    for (size_t i = 0; i < out.total_values(); ++i) {
        double v = out.pixels()[i];
        out.pixels()[i] = v > 0 ? std::pow(v, g_) : 0;
    }
    return out;
}
std::string GammaOperator::name() const { return "Gamma (g=" + std::to_string(g_).substr(0, 4) + ")"; }

Image ThresholdOperator::apply(const Image& in) const {
    Image out = in.clone();
    for (size_t i = 0; i < out.total_values(); ++i) out.pixels()[i] = out.pixels()[i] >= t_ ? 1.0 : 0.0;
    return out;
}
std::string ThresholdOperator::name() const { return "Threshold (t=" + std::to_string(t_).substr(0, 4) + ")"; }

Image GrayscaleOperator::apply(const Image& in) const {
    if (in.channels() == 1) return in.clone();
    Image out(in.rows(), in.cols(), 1);
    for (int i = 0; i < in.rows(); ++i) {
        for (int j = 0; j < in.cols(); ++j) {
            double v = 0.2125 * in.at(i, j, 0) + 0.7154 * in.at(i, j, 1) + 0.0721 * in.at(i, j, 2);
            out.set(i, j, v);
        }
    }
    return out;
}
std::string GrayscaleOperator::name() const { return "Grayscale"; }

Image GaussianNoiseOperator::apply(const Image& in) const {
    Rng rng(seed_++);
    Image out = in.clone();
    for (size_t i = 0; i < out.total_values(); ++i) out.pixels()[i] += sigma_ * rng.normal();
    return out;
}
std::string GaussianNoiseOperator::name() const { return "Gaussian noise (sigma=" + std::to_string(sigma_).substr(0, 4) + ")"; }

Image SaltPepperOperator::apply(const Image& in) const {
    Rng rng(seed_++);
    Image out = in.clone();
    for (size_t i = 0; i < out.total_values(); ++i) {
        double u = rng.uniform();
        if (u < p_ / 2) out.pixels()[i] = 0.0;
        else if (u < p_) out.pixels()[i] = 1.0;
    }
    return out;
}
std::string SaltPepperOperator::name() const { return "Salt & pepper (p=" + std::to_string(p_).substr(0, 4) + ")"; }

Image FlipHorizontalOperator::apply(const Image& in) const {
    Image out = in.clone();
    for (int ch = 0; ch < in.channels(); ++ch)
        for (int i = 0; i < in.rows(); ++i)
            for (int j = 0; j < in.cols(); ++j)
                out.set(i, in.cols() - 1 - j, in.at(i, j, ch), ch);
    return out;
}
std::string FlipHorizontalOperator::name() const { return "Flip horizontal"; }

Image FlipVerticalOperator::apply(const Image& in) const {
    Image out = in.clone();
    for (int ch = 0; ch < in.channels(); ++ch)
        for (int i = 0; i < in.rows(); ++i)
            for (int j = 0; j < in.cols(); ++j)
                out.set(in.rows() - 1 - i, j, in.at(i, j, ch), ch);
    return out;
}
std::string FlipVerticalOperator::name() const { return "Flip vertical"; }

// ======================= IO and App =======================
Image ImageLoader::load(const std::string& filename) {
    int w = 0, h = 0, comp = 0;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &comp, 0);
    if (!data) throw std::runtime_error("ImageLoader: cannot load '" + filename + "'");
    
    Image img;
    if (comp >= 3) {
        img = Image(h, w, 3);
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j) {
                const unsigned char* p = data + ((size_t)i * w + j) * comp;
                img.set(i, j, p[0] / 255.0, 0);
                img.set(i, j, p[1] / 255.0, 1);
                img.set(i, j, p[2] / 255.0, 2);
            }
    } else {
        img = Image(h, w, 1);
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j)
                img.set(i, j, data[(size_t)i * w + j] / 255.0);
    }
    stbi_image_free(data);
    return img;
}

void ImageSaver::save(const std::string& filename, const Image& img) {
    std::vector<unsigned char> buf((size_t)img.rows() * img.cols() * 3);
    for (int i = 0; i < img.rows(); ++i) {
        for (int j = 0; j < img.cols(); ++j) {
            size_t off = ((size_t)i * img.cols() + j) * 3;
            auto to_byte = [](double x) {
                if (!(x > 0)) return (unsigned char)0;
                if (x >= 1) return (unsigned char)255;
                return (unsigned char)std::lround(x * 255);
            };
            if (img.channels() == 1) {
                unsigned char v = to_byte(img.at(i, j));
                buf[off] = buf[off + 1] = buf[off + 2] = v;
            } else {
                buf[off]     = to_byte(img.at(i, j, 0));
                buf[off + 1] = to_byte(img.at(i, j, 1));
                buf[off + 2] = to_byte(img.at(i, j, 2));
            }
        }
    }
    std::string ext = filename.substr(filename.find_last_of('.') + 1);
    int ok = (ext == "jpg" || ext == "jpeg") 
        ? stbi_write_jpg(filename.c_str(), img.cols(), img.rows(), 3, buf.data(), 92)
        : stbi_write_png(filename.c_str(), img.cols(), img.rows(), 3, buf.data(), img.cols() * 3);
    if (!ok) throw std::runtime_error("ImageSaver: cannot write '" + filename + "'");
}

std::string AsciiPreview::render(const Image& img) const {
    static const char* ramp = " .:-=+*#%@";
    if (img.rows() == 0 || img.cols() == 0) return "";
    int W = width_;
    int H = std::max(1, (int)std::lround(W * (double)img.rows() / img.cols() / 2.0));
    int L = (int)std::strlen(ramp);
    std::ostringstream out;
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int r0 = i * img.rows() / H, r1 = std::max(r0 + 1, (i + 1) * img.rows() / H);
            int c0 = j * img.cols() / W, c1 = std::max(c0 + 1, (j + 1) * img.cols() / W);
            double s = 0; int n = 0;
            for (int r = r0; r < r1 && r < img.rows(); ++r)
                for (int c = c0; c < c1 && c < img.cols(); ++c) {
                    double v = 0;
                    for (int ch = 0; ch < img.channels(); ++ch) v += img.at(r, c, ch);
                    s += v / img.channels(); ++n;
                }
            double v = n ? s / n : 0;
            out << ramp[std::max(0, std::min(L - 1, (int)(v * L)))];
        }
        out << "\n";
    }
    return out.str();
}

// ======================= Menu & App =======================
Menu& Menu::add(std::string label, std::function<void()> action) {
    items_.emplace_back(std::move(label), std::move(action));
    return *this;
}

void Menu::run() const {
    while (true) {
        std::cout << "\n=== " << title_ << " ===\n";
        for (size_t i = 0; i < items_.size(); ++i) std::cout << "  " << (i + 1) << ") " << items_[i].first << "\n";
        std::cout << "  0) Back\n> ";
        int c;
        if (!(std::cin >> c)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (c == 0) return;
        if (c >= 1 && c <= (int)items_.size()) {
            try { items_[c - 1].second(); }
            catch (const std::exception& e) { std::cerr << "Error: " << e.what() << "\n"; }
        } else std::cout << "Invalid choice.\n";
    }
}

void ImageEditorApp::run() {
    if (!prompt_load()) { std::cout << "No image loaded. Bye.\n"; return; }
    original_ = current_;
    main_menu().run();
}

bool ImageEditorApp::prompt_load() {
    std::cout << "=== OOP Image Editor ===\nPath to image (png/jpg, blank to cancel): ";
    std::string s; std::getline(std::cin, s);
    if (s.empty()) return false;
    try {
        current_ = ImageLoader::load(s);
        filename_ = s;
        std::cout << "Loaded '" << s << "' (" << current_.rows() << "x" << current_.cols() << ", " << current_.channels() << " channel(s))\n";
        return true;
    } catch (const std::exception& e) { std::cerr << "Error: " << e.what() << "\n"; return false; }
}

int ImageEditorApp::prompt_int(const std::string& p, int lo, int hi) {
    while (true) {
        std::cout << p << " [" << lo << "-" << hi << "]: ";
        int v;
        if (std::cin >> v && v >= lo && v <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return v;
        }
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  enter a number between " << lo << " and " << hi << "\n";
    }
}

double ImageEditorApp::prompt_double(const std::string& p, double lo, double hi) {
    while (true) {
        std::cout << p << " [" << lo << "-" << hi << "]: ";
        double v;
        if (std::cin >> v && v >= lo && v <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); return v;
        }
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  enter a number between " << lo << " and " << hi << "\n";
    }
}

std::string ImageEditorApp::prompt_line(const std::string& p) {
    std::cout << p; std::string s; std::getline(std::cin, s); return s;
}

int ImageEditorApp::odd_kernel(const std::string& p, int lo, int hi) {
    int k = prompt_int(p, lo, hi);
    if (k % 2 == 0) ++k;
    return k;
}

void ImageEditorApp::apply(std::unique_ptr<ImageOperator> op) {
    try {
        Image result = op->apply(current_);
        result.clamp(0.0, 1.0);
        history_.push_back({op->name(), current_});
        current_ = result;
        std::cout << "Applied: " << op->name() << "\n";
    } catch (const std::exception& e) { std::cerr << "Error: " << e.what() << "\n"; }
}

Menu ImageEditorApp::main_menu() {
    Menu m("Main Menu");
    m.add("Show image info", [this]{ info(); })
     .add("ASCII preview", [this]{ preview(); })
     .add("Filters (blur / sharpen / edges)", [this]{ filters_menu().run(); })
     .add("Point ops (invert / gamma / threshold)", [this]{ point_menu().run(); })
     .add("Grayscale", [this]{ apply(std::make_unique<GrayscaleOperator>()); })
     .add("Noise", [this]{ noise_menu().run(); })
     .add("Flip horizontal", [this]{ apply(std::make_unique<FlipHorizontalOperator>()); })
     .add("Flip vertical", [this]{ apply(std::make_unique<FlipVerticalOperator>()); })
     .add("Save current image", [this]{ save_image(); })
     .add("Show edit history", [this]{ show_history(); })
     .add("Undo last edit", [this]{ undo(); })
     .add("Reset to original", [this]{ current_ = original_; std::cout << "Reset.\n"; });
    return m;
}

Menu ImageEditorApp::filters_menu() {
    Menu m("Filters");
    m.add("Gaussian blur", [this]{
        int k = odd_kernel("Kernel size", 3, 31);
        double s = prompt_double("Sigma", 0.1, 10.0);
        apply(std::make_unique<GaussianBlurOperator>(k, s));
    }).add("Box blur", [this]{
        int k = odd_kernel("Kernel size", 3, 31);
        apply(std::make_unique<BoxBlurOperator>(k));
    }).add("Sharpen", [this]{ apply(std::make_unique<SharpenOperator>()); })
      .add("Median filter", [this]{
        int k = odd_kernel("Kernel size", 3, 15);
        apply(std::make_unique<MedianFilterOperator>(k));
    }).add("Sobel edges", [this]{ apply(std::make_unique<SobelOperator>()); });
    return m;
}

Menu ImageEditorApp::point_menu() {
    Menu m("Point Operations");
    m.add("Invert", [this]{ apply(std::make_unique<InvertOperator>()); })
     .add("Gamma correction", [this]{
        double g = prompt_double("Gamma", 0.1, 5.0);
        apply(std::make_unique<GammaOperator>(g));
    }).add("Threshold", [this]{
        double t = prompt_double("Threshold", 0.0, 1.0);
        apply(std::make_unique<ThresholdOperator>(t));
    });
    return m;
}

Menu ImageEditorApp::noise_menu() {
    Menu m("Noise");
    m.add("Add Gaussian noise", [this]{
        double s = prompt_double("Sigma", 0.001, 0.5);
        apply(std::make_unique<GaussianNoiseOperator>(s));
    }).add("Add salt & pepper", [this]{
        double p = prompt_double("Probability", 0.0, 1.0);
        apply(std::make_unique<SaltPepperOperator>(p));
    });
    return m;
}

void ImageEditorApp::info() const {
    std::cout << "Dimensions: " << current_.rows() << " x " << current_.cols() << "\n"
              << "Channels:   " << current_.channels() << "\n"
              << "Min/Max/Mean: " << current_.min_value() << " / "
              << current_.max_value() << " / " << current_.mean_value() << "\n"
              << "Edits so far: " << history_.size() << "\n";
}

void ImageEditorApp::preview() const { std::cout << AsciiPreview(60).render(current_); }

void ImageEditorApp::save_image() {
    std::string name = prompt_line("Output filename (e.g., out.png): ");
    if (name.empty()) return;
    try { ImageSaver::save(name, current_); std::cout << "Saved to " << name << "\n"; }
    catch (const std::exception& e) { std::cerr << "Error: " << e.what() << "\n"; }
}

void ImageEditorApp::show_history() const {
    if (history_.empty()) { std::cout << "(no edits yet)\n"; return; }
    for (size_t i = 0; i < history_.size(); ++i)
        std::cout << "  " << (i + 1) << ") " << history_[i].first << "\n";
}

void ImageEditorApp::undo() {
    if (history_.empty()) { std::cout << "Nothing to undo.\n"; return; }
    current_ = history_.back().second;
    std::cout << "Undid: " << history_.back().first << "\n";
    history_.pop_back();
}

void printMenu() {
    std::cout << "\n=== Image Editor Main Menu ===\n";
    std::cout << "1. Invert Image\n";
    std::cout << "2. Gaussian Blur\n";
    std::cout << "3. Sharpen Image\n";
    std::cout << "4. Exit Program\n";
    std::cout << "==============================\n";
}
int getUserMenuOption() {
    int option;
    std::cout << "Enter choice (1-4): ";
    if (!(std::cin >> option)) {
     
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        return -1; 
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return option;
}

int validateMenuOption(int option, int min, int max) {
    if (option >= min && option <= max) {
        return option;
    }
    return -1; // Invalid
}


void runInvert(Image& img) {
    InvertOperator op;
    img = op.apply(img);
    img.clamp(0.0, 1.0);
    std::cout << "[Operation Applied]: " << op.name() << "\n";
}

void runGaussianBlur(Image& img) {
    GaussianBlurOperator op(5, 1.5);
    img = op.apply(img);
    img.clamp(0.0, 1.0);
    std::cout << "[Operation Applied]: " << op.name() << "\n";
}

void runSharpen(Image& img) {
    SharpenOperator op;
    img = op.apply(img);
    img.clamp(0.0, 1.0);
    std::cout << "[Operation Applied]: " << op.name() << "\n";
}