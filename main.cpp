#include <iostream>
#include "krylov_tools.hpp"

int main() {
    try {
        ImageEditorApp app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}