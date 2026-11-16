# Checkpoint Development Portfolio - CSC 1060 Final Project

## Purpose and Requirements

This project is developed incrementally through checkpoints. Each checkpoint focuses on a specific stage of software development and builds on the work completed previously.

## Important GitHub Requirements

For every checkpoint, follow this sequence:

1. Complete the assigned work: Development Task.
2. Test the changes.
3. Update the reflection file: Reflection
4. Commit and sync using the exact message provided: Commit and Sync
5. Open your GitHub repo and verify that the remote repository contains the latest changes.

>
> A checkpoint is not complete until its development work and reflection have both been committed and synchronized to GitHub.
>
> Your GitHub repository must demonstrate your progress throughout development. Do **NOT** wait until the end of the entire project to commit and sync your work!
> 

---

# Checkpoint 01: Initial Project Structure / Setup

## Development Tasks

1. Add a new file to the project named: `checkpoint_reflection.md`
  - Save the file in the project's root repository folder.
2. At the top of the file include:
  - Your Name
3. Add a new file to the project named: `main.cpp`
4. Write the code for `Hello World`
5. Compile and Run the program using `g++`
6. Open `.vscode` folder and update `tasks.json`
    - Update line 10 to: `"*.cpp",`
7. Add additional files needed for the project.
    - Class files (header .h and source .cpp)
        - **NO** code yet, the files can be empty
    - Data Files (.txt, .dat, or .csv)
        - **NO** data needed yet - the file can be empty
8. Compile and run the program
9. Add a new file to the project named: `checkpoint_reflection.md` (keep the formatting as shown in the markdown file)
    - Replace the [Your Name], [Project Title], [Project Summary] with information about you and your program

## Reflection

```

# [Your Name]
## [Project Title]
## [Project Summary]

---

## checkpoint-01: Initial project structure / setup

1. What challenges did you encounter?
2. What design decisions or changes did you make?

```

## Commit and Sync Checkpoint 01

>
> **Commit and sync message:** `checkpoint-01: initial project structure / setup`
>

---

# Checkpoint 02: Class Declarations

## Development Tasks

1. Using your UML class designed from Final Project Phase II Design
2. Declare your class in the header file
    - Connect the class files with includes and macro guard
    - Add a comment summary at the top of the header:
     - Your Name
     - Summary of the Class
3. Apply appropriate class abstraction and encapsulation access specifiers, such as `private` and `public`.
4. Use appropriate data types for the class members.
5. Create multiple constructors, setters, getters, and other member function declarations
    - **Note**: in-line function definitions are allowed for functions that have a single code instruction.
    - Function definitions will be completed in the next checkpoint 
6. Ensure that the declarations reflect the design of your program.
7. Compile and run the program
    - **Note**: Program most likely will not compile as the function definitions are missing
8. Update `checkpoint_reflection.md` by answering the following questions (keep the formatting as shown in the markdown file)

## Reflection

```

---

## checkpoint-02: class declaration

1. What challenges did you encounter?
2. What design decisions or changes did you make?

```

## Commit and Sync Checkpoint 02

>
> **Commit and sync message:** `checkpoint-02: class declaration`
>

---

# Checkpoint 03: Class Definition

## Development Tasks

1. Define each member function from the class declaration in the corresponding source file
    - Add a comment summary at the top of the header:
        - Your Name
    - Ensure definition file has included the corresponding source file
    - Ensure function definitions match their declarations
    - Ensure the definitions focus on data encapsulation and completing one task.
    - Use appropriate scope resolution to define class member functions outside the class declaration.
2. Perform a unit test on each member function definition by calling each function in `main.cpp` to verify the results and functionality
   - Test class constructors and member functions.
   - Verify that objects can access their intended public member functions.
   - Confirm that the class implementation behaves as expected, even though the full program functionality is not yet complete.
3. Compile and run the program
4. Rename `main.cpp` as `maintest.txt`
    - The file extension **MUST** be renamed from `.cpp` to `.txt` so that the program is no longer compiled with the program!
5. Update `checkpoint_reflection.md` by answering the following questions (keep the formatting as shown in the markdown file)

## Reflection

```

---

## checkpoint-03: class definition

1. What challenges did you encounter?
2. What design decisions or changes did you make?

```

## Commit and Sync Checkpoint 03

>
> **Commit and sync message:** `checkpoint-03: class definition`
>

---

# Checkpoint 04: Program Structure Integration: Main Menu and User Input Validation

## Development Tasks

1. Add a new `main.cpp` as the previous main is now a unit test and was renamed
    - Add a comment at the top with:
        - Your Name
        - Program Title
        - Program Summary
    - Add all includes needed for the program 
2. Declare the functions needed to support the main menu and program operations.
    - Example of support functions: `getUserMenuOption`, `validateMenuOption`, `printMenu`, ...
3. Place function prototypes in appropriate header files
4. Define the stand-alone functions in the appropriate source files
    - Ensure function headers match the intended parameters and return types.
5. Implement menu structure in `main.cpp` with user interaction that displays the available program options.
    - Allow the user to select a menu option.
    - Include an option to exit the program.
    - Ensure the menu displays correctly
6. Connect menu selections to the appropriate stand-alone functions.
    - Do **NOT** connect File IO during this checkpoint as it will be connected later
    - Confirm that the user can perform multiple operations during a program session.
    - Ensure input validation is handled correctly
    - Handle invalid data and unexpected input appropriately.
    - Prevent invalid input from causing incorrect program behavior.
7. Compile and run the program
8. Update `checkpoint_reflection.md` by answering the following questions (keep the formatting as shown in the markdown file)

## Reflection

```

---

## checkpoint-04: program structure integration and input validation

1. What challenges did you encounter?
2. What design decisions or changes did you make?

```

## Commit and Sync Checkpoint 04

>
> **Commit and sync message:** `checkpoint-04: program structure integration and input validation`
>

---

# Checkpoint 05: Incorporate Objects into Program Flow

## Development Tasks

1. Create the objects needed for the program.
2. Initialize objects appropriately.
3. Use objects and member functions to perform the required operations from the menu.
    - Ensure each menu option invokes the correct functionality.
4. Test to verify that each implemented operation works correctly.
5. Compile and run the program
6. Update `checkpoint_reflection.md` by answering the following questions (keep the formatting as shown in the markdown file)

## Reflection

```

---

## checkpoint-05: incorporate objects into program flow

1. What challenges did you encounter?
2. What design decisions or changes did you make?

```

## Commit and Sync Checkpoint 05

>
> **Commit and sync message:** `checkpoint-05: incorporate objects into program flow`
>

---

# Checkpoint 06: File IO / Data Persistence

## Development Tasks

1. Update data files `.dat, .txt, .csv` with real data
2. Create either stand-alone functions or member functions within classes to perform File IO:
    - Read (Load) existing program data from file or files into program.
    - Write (Save) program data to the required file or files.
3. Call the File IO functions within the main program structure.
    - Integrate file operations with all required program features.
4. Verify that saved data can be retrieved during a subsequent program execution.
5. Handle missing or empty data files as appropriate.
6. Test to verify that each implemented operation works correctly.
7. Compile and run the program
8. Update `checkpoint_reflection.md` by answering the following questions (keep the formatting as shown in the markdown file)

## Reflection

```

---

## checkpoint-06: file io / data persistence

1. What challenges did you encounter?
2. What design decisions or changes did you make?

```

## Commit and Sync Checkpoint 06

>
> **Commit and sync message:** `checkpoint-06: file io / data persistence`
>

---

# Checkpoint 07: Final Testing and Program Completion, Verification, and Submission

## Development Tasks

1. Verify that the program compiles successfully.
2. Confirm that all menu options work as intended.
3. Ensure that classes, functions, and objects behave correctly.
4. Verify that file operations and data persistence work as expected.
5. Resolve any remaining compilation errors or significant functional problems.
6. Test all major program features.
    - Test valid and invalid user input.
    - Test file loading and saving.
    - Verify that records can be added, displayed, searched, and updated, as applicable.
7. Review code organization, readability, naming, and consistency.
8. Correct any remaining defects.
9. Confirm that the completed program meets the requirements of your project proposal.
10. Confirm that your commit history shows the required incremental development.
    - Ensure all completed work has been pushed and synchronized to GitHub.
    - Verify that the remote repository contains your final working version.
    - Confirm that all required source files, header files, and data files are included in the repository.

## Final Reflection

```

---

## checkpoint-07: final testing and program completion, verification, and submission

1. What were the most significant challenges you encountered, and how did you resolve them?
2. What were the most significant design decisions or changes did you make throughout development?
3. What did you learn about developing a C++ program incrementally?
4. Are there any known issues, limitations, or incomplete features?
5. How does your completed program compare with your original program summary and design?

---

```

## Commit and Sync Checkpoint 07

>
> **Commit and sync message:** `checkpoint-07: final testing and program completion, verification, and submission`
>

---

# Final Submission Requirements

Your remote GitHub repository must contain:

- The completed C++ program.
- All required source and header files.
- Any required data files.
- Reflection file updated from checkpoint 01 to 07.
- A commit history showing the required development parts were committed and synchronized throughout the project.

---

