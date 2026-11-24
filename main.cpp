/*
Name: Nicholas Orndoff
Program Title: Image Editor - Checkpoint 06
Program Summary: Integrates File IO into the program flow. The ImageLoader and 
ImageSaver classes are called from the menu to read and write image data. 
Includes validation to prevent operations on unloaded images and handles 
missing file exceptions gracefully.
*/

/*
if you want to run full program 

int main() {
    try {
        ImageEditorApp app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

*/

#include <iostream>
#include <string>
#include "krylov_tools.hpp"


int main() {
    // Start with an empty image object
    Image currentImage;
    bool imageLoaded = false; 
    
    std::cout << "Welcome to the Image Editor!\n";

    bool running = true;

    while (running) {
        printMenu();
        
        int rawInput = getUserMenuOption();
        int validChoice = validateMenuOption(rawInput, 1, 6); // Updated max to 6

        if (validChoice == -1) {
            std::cout << "ERROR: Invalid input! Please enter a number from 1 to 6.\n";
            continue;
        }

        switch (validChoice) {
            case 1: { // LOAD IMAGE
                std::cout << "Enter filename to load (e.g., test.png): ";
                std::string filename;
                std::getline(std::cin, filename);
                
                try {
                    // Call the OOP File IO member function
                    currentImage = ImageLoader::load(filename);
                    imageLoaded = true;
                    std::cout << "SUCCESS: Loaded " << filename << " (" 
                              << currentImage.rows() << "x" << currentImage.cols() << ")\n";
                } catch (const std::exception& e) {
                    std::cout << "FILE ERROR: " << e.what() << "\n";
                    std::cout << "(Please check the filename and try again).\n";
                    imageLoaded = false;
                }
                break;
            }
            case 2: { // SAVE IMAGE
                if (!imageLoaded) {
                    std::cout << "ERROR: You must load an image before saving.\n";
                    break;
                }
                std::cout << "Enter filename to save as (e.g., output.png): ";
                std::string filename;
                std::getline(std::cin, filename);
                
                try {
                    // Call the OOP File IO member function
                    ImageSaver::save(filename, currentImage);
                    std::cout << "SUCCESS: Saved image to " << filename << "\n";
                } catch (const std::exception& e) {
                    std::cout << "FILE ERROR: " << e.what() << "\n";
                }
                break;
            }
            case 3: { // INVERT
                if (!imageLoaded) {
                    std::cout << "ERROR: Please load an image first (Option 1).\n";
                    break;
                }
                InvertOperator op; 
                currentImage = op.apply(currentImage); 
                currentImage.clamp(0.0, 1.0);
                std::cout << "[Applied]: " << op.name() << "\n";
                break;
            }
            case 4: { // BLUR
                if (!imageLoaded) {
                    std::cout << "ERROR: Please load an image first (Option 1).\n";
                    break;
                }
                GaussianBlurOperator op(5, 1.5); 
                currentImage = op.apply(currentImage);
                currentImage.clamp(0.0, 1.0);
                std::cout << "[Applied]: " << op.name() << "\n";
                break;
            }
            case 5: { // SHARPEN
                if (!imageLoaded) {
                    std::cout << "ERROR: Please load an image first (Option 1).\n";
                    break;
                }
                SharpenOperator op;
                currentImage = op.apply(currentImage);
                currentImage.clamp(0.0, 1.0);
                std::cout << "[Applied]: " << op.name() << "\n";
                break;
            }
            case 6:
                std::cout << "Exiting program. Goodbye!\n";
                running = false;
                break;
        }
    }

    return 0;
}