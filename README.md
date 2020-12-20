Assumptions:
1) Input file name will always be provided. For simplicity, I have taken input.txt as input file name.
2) Input will be such that every node of the tree will either have 2 childs (left and right) or 0 child (leaf nodes). No node will have only one child.
3) All weight numbers are integers(standard int)
4) All inputs in the file will be provided in sequence so that (level order from left to right)


Basic Test file inputs:
#ScaleName,Left,Right
B1,10,B2
B2,B3,4
B3,7,8

Compilation and Application Usage:
For simplicity, I have kept most of the things in header file so we will have to just compile main source file containing main function
Compiler Version: 
$ g++ --version
g++ (GCC) 4.9.1 20140922 (Red Hat 4.9.1-10)

Application Compilation:
$ g++ application_main.cpp -std=c++14 -o BalancingScale

Application Execution:
$ ./BalancingScale input.txt 

Output to console:
$ ./BalancingScale input.txt
B1,25,0
B2,0,13
B3,1,0

