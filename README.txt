/*////////////////////////////////////////////////
CS 537 Prog 3
Authors: Peyton Gardipee
         Tushar Verma
File: README        
*////////////////////////////

In this program our aim is to build a program that can parse a makefile
and successfully build an executable. The program looks for a makefile,
by default, if not found it looks for Makefile. If a filename has been 
passed as an argument using a -f flag then it parses that file. After 
parsing is done,the program builds a graph. It does this by making each 
target into a node of type targetNode which is of type struct. Each target 
node maintains a list of its dependencies and the commands associated with it. 
We used linked lists to implement this data structure. Additionaly, you
can only run a specific traget by passing the name of the target as the
argument. If a makefile has any cycles in any graphs then the program prints
cycle detected and exits.