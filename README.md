# Assembler

Final project in a laboratory course in systems programming.
You can find a link to the full project here: https://github.com/noa207207/Assembler

The program realizes the translation of assembly code into a unique binary language.
The program is sensitive to errors, so in the event of an error the software will display an error message and will not create an object file.
But, even when errors are found, the software continues to go over the rest of the file and checks its integrity.

how to use: 
1. Compile the program using the make command. (Since the folder contains a compiled file that will not match all versions of C, sometimes it is necessary to run make clean to clean the files from the last compilation, and compile again using the make command)

2. Run the program and insert the file relative links as arguments (without the extension).

USAGE:

make clean

make

./assembler test_files/test1
