# [Nicholas Orndoff]
## [Image Filter]
## [An application that can allow you to:
## - Load an Image
## - Apply a variety of filters to Image
## - View the Image in ASCII
## - Flip the Image?
## - View the Edit History
## - Save the Image]

---

## checkpoint-01: Initial project structure / setup

1. What challenges did you encounter?
        I encountered trouble with some of the git stuff. If I'm being totally honest, I'm not sure it's totally correct. I did clone the correct repo, but it wouldn't let me do it without uploading this repo to my public git account. I don't know if this changes the way it syncs with the classroom but we will find out later I guess.
2. What design decisions or changes did you make?
        I decided that I would need to restructure and rethink my main menu, as well as solidify everything into one header file.

---

## checkpoint-02: class declaration

1. What challenges did you encounter?
        I had a hard time with refactoring the code I used in my linear algebra class to fit the OOP program structure needed for this class. I also had a hard time figuring out git because it wasn't pushing or staging any files for a minute. I think I figured it out but we will see.
2. What design decisions or changes did you make?
        I decided that I would include all of my operations in one helper file for this assignment, as well as changing the logic for some of the functions.

---

## checkpoint-03: class definition

1. What challenges did you encounter?
        Same things as in my 2nd checkpoint, the reason being that I sorta did this in a different way than you went about it but mostly due to the issues I was having with git.
2. What design decisions or changes did you make?
        same thing


---

## checkpoint-04: program structure integration and input validation

1. What challenges did you encounter?
        I did not encounter much of a challenge with this, I just had to adjust some of the things I did to meet the requirements for this checkpoint.
2. What design decisions or changes did you make?
        I implemented the user interface and input validation as stand-alone procedural functions. To maintain my original software architecture, i wrote procedural wrapper functions that instantiate my objects and apply them to the object passed by reference.


---

## checkpoint-06: file io / data persistence

1. What challenges did you encounter?
        The biggest issue I found with this whole project was implementing the Image saving. I used the same stb_image libraries that we used in my linear algebra class, but it still posed challenging for me.
2. What design decisions or changes did you make?
        I chose to use my ImageLoader::load function to check for if a user attempts to load a fil that does not exist, and throws a runtime error back. In the main.cpp, the load operation is wrapped in a try/catch block so if the file is missing, the catch block can keep the program running without crashing.


---

## checkpoint-07: final testing and program completion, verification, and submission

1. What were the most significant challenges you encountered, and how did you resolve them?
        I think that the biggest challenge was refactoring the original program I had written before, but just navigating the requirements between the checkpoints in between was just as difficult. I had made the same crucial error that I keep making which is not fully looking at the assignment details, and to make matters worse, I had issues uploading things to git. I managed to figure them out but I still had originally built a fully encapsulated Object Oriented menu system. I Implemented a Hybrid architecture in main.cpp that uses procedural functions and a switch statement, but it still interacts with my OOP backend. Another major challenge was linking the third party stb image libraries; I resolved the "multiple definition" linker errors by isolating the 'STB_IMAGE_IMPLEMENTATION macro into 1 source file.

2. What were the most significant design decisions or changes did you make throughout development?
        I think that the most significant design decision was that instead of writing a massive procedural function with if/else statements for every filter, I made an abstract class with an apply() pure virtual function.This was the ImageOperator abstract base class. It allowed me to add 12 filters by just making subclasses. The menu can treat all filters identically through polymorphism.

3. What did you learn about developing a C++ program incrementally?
        I learned the value of separating declaration from definition. It made me really appreciate the value of a clean workspace (even though mine is sort of cluttered, its only because I couldn't figure out how to clean things up after rebasing and I don't want to mess anything else up)
4. Are there any known issues, limitations, or incomplete features?
        The biggest limitation is the final integration menu, it only exposes 3 filters while my OOP backend supports 12 filters but trying to stay more accurate to the rubric, only 3 are wired into the switch statement. Also, because its a terminal based app, all the image processing is done in memory without a GUI. Big images might take a moment to process depending on the kernel size.
5. How does your completed program compare with your original program summary and design?
        I think that my completed program strongly alligns with my original program summary, although I'm not too sure about design. I wish to go back and fix that if I could, even if it's not for points.

   I know that I did not do checkpoint 5, but it's only because I felt that I was beyond that one at the point and it would be more work to somehow demonstrate it in a commit rather than to just turn it in, especially when I basically already had my entire project ready at that point. I apologize and I hope you can understand.     

---





