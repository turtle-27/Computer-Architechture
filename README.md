# Computer-Architechture
These are the COL216 Computer Architecture course assignments. A1 and A3 are in MIPS assembly language.  Minor, A3, A4, and A5 are implemented in C++ programming language. 

## Assignment 1
Using a MIPS Assembly Program, calculate the area under a curve made by connecting a series of points with a straight line.

## Assignment 2
Postfix expression evaluator using MIPS Assembly Program

## Assignment 3
Implementation of an interpreter to handle a section of MIPS assembly language commands. Line by line, instruction is read and executed from an input file.

## Minor
Two new features are added to the interpreter created in A3 to transform it into a MIPS simulator.
 - Designing 2D-DRAM
 - Non-blocking Memory Access

## Assignment 4
The DRAM may change rows ON EVERY ACCESS if we handle memory requests in the input order, which has a negative impact on performance. 

The MIPS Simulator is used in this assignment to perform memory request ordering in order to boost performance.

## Assignment 5
The earlier designed DRAM request manager is expanded to the multicore CPU scenario in this assignment. A Memory Request Manager, which communicates with the DRAM, receives DRAM requests from N CPU cores, each of which is running a distinct MIPS application.
