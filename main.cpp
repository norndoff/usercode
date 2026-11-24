/*
 * Name: Nicholas Orndoff
 * Program Title: Image Editor 
 * Program Summary: Procedural implementation of the main menu and input validation.
 * This connects to the OOP backend, but intentionally omits File IO.
 */

#include <iostream>
#include "krylov_tools.hpp"


int main() {
    // Create a dummy image in memory
    Image currentImage(100, 100, 1);
    
    std::cout << "Welcome to the Image Editor!\n";
    std::cout << "(A blank 100x100 image has been initialized in memory).\n";

    bool running = true;

    while (running) {
        printMenu();
        
        int rawInput = getUserMenuOption();
        int validChoice = validateMenuOption(rawInput, 1, 4);

        if (validChoice == -1) {
            std::cout << "ERROR: Invalid input! Please enter a number from 1 to 4.\n";
            continue;
        }

        switch (validChoice) {
            case 1:
                runInvert(currentImage);
                break;
            case 2:
                runGaussianBlur(currentImage);
                break;
            case 3:
                runSharpen(currentImage);
                break;
            case 4:
                std::cout << "Exiting program. Goodbye!\n";
                running = false;
                break;
        }
    }

    return 0;
}