#include <iostream>
#include "krylov_tools.hpp"

int main() {
    Image img(2, 3, 1);
    img.set(0, 0, 0.5);
    return img.at(0, 0) == 0.5 ? 0 : 1;
}