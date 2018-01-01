# CPSL Compiler
This compiler was written to gain experience with compilers. This compiler takes instructions written in the CPSL language and compiles them down to MIPS assembly.

CPSL (the Compiler Project Source Language) is a Pascal-like language. It was developed specifically to be easy to compile and is a great first language for writing compilers. For more information about the CPSL language, see the language's specification at https://github.com/ksundberg/CS5300/blob/master/CPSL.tex or, for the version used when writing this compiler, see the included latex file CPSL.tex.

## Dependencies
gcc 4.8.1
flex 2.5.4
bison 2.5
cmake

## How to use

### Build

To build the compiler, run the commands:
> cmake CMakeLists.txt
> make

This will produce an executable named "cpsl".

### Input

The compiler must recieve the name of a cpsl file as an inline argument.
> ./cpsl filename

### Output

The compiled program will be written to a file called "output.asm". The console output of the program is for debugging purposes.


## Notes

All features required for the full CPSL language have not been implemented. Below are a list of features have and have not been implemented.

Working perfectly:
* Scanner
* Parser
* Expressions
* Control Flow

Partly working:
* Functions. Arrays work, but Records do not. Arrays also have a known bug that prevent them from being copied.

Not working:
* User Defined Data Types
