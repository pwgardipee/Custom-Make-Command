/*////////////////////////////
CS 537 Prog 3
Authors: Peyton Gardipee
         Tushar Verma
File: fileparser.c        
*////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "graphNode.h"
#include "constants.h"

targetNode* fileParser(char* customFileName)
{
    char *ch = (char*)calloc(1,sizeof(char));
	int startLine, index, commandIndex, commandLineIndex;
    targetNode *headNode, *currTarget;
    dependencyNode *currDependency, *tempDependency;
    char *buffer = (char*)calloc(BUFF_SIZE,sizeof(char));
    char *dependencyToken, *commandToken;
    commandLineIndex = 0;
    int lineNumber = 1;


    FILE *fptr;

    if(customFileName != NULL)
    {
        //If a custom file is used with -f then check for it
        fptr = fopen(customFileName, "r");

        if(fptr == NULL)
        {
            fprintf(stderr, "File does not exist\n");
            exit(EXIT_FAILURE);
        }

    }
    else{
        fptr = fopen("makefile", "r");
        
        //Check for both valid file names. If neither exist, exit prog.
        if(fptr == NULL)
        {
            //"makefile" doesn't exist. Check to see if "Makefile" exists
            fptr = fopen("Makefile", "r");
            if(fptr ==  NULL)
            {
                //"Makefile" doesn't exist either. Exit from prog.
                fprintf(stderr, "File does not exist\n");
                exit(EXIT_FAILURE);
            }
        }

    }

    

    //Initialize space for a target node which will be the head of the list of target nodes
    currTarget = malloc(sizeof(targetNode));
    currTarget->commands = malloc(SIZE * sizeof(char**));
    currTarget->firstDependency = NULL;
    currTarget->nextTarget = NULL;
    currTarget->targetName = NULL;
    headNode = currTarget;

    //Upon getting the first char of a file, we are at the start of a line
	startLine = 1;

    //Iterate through each character of the file
	while((*ch = fgetc(fptr)) != EOF){
        if(*ch == '\n')
		{
            //If we have reached the end of a line, then the next character will be at the start of a new line
            startLine = 1;
            lineNumber++;
		}
		else
		{
            //If we are at the start of a line, check if this line is a target, command, comment, or invalid
            if(startLine)
            {
                //If the first character is a tab, then the line is a command
                if(*ch == '\t')
                {
                    //flush out tab character
                    *ch = fgetc(fptr);


                    //Create space for a new line that represents a command
                    currTarget->commands[commandLineIndex] = malloc(SIZE * sizeof(char*));

                    //Put rest of line in a buffer
                    index = 0;
                    memset((void*)buffer, 0, BUFF_SIZE);
                    while((*ch != EOF) && (*ch != '\n'))
                    {
                        if(index > BUFF_SIZE)
                        {
                            fprintf(stderr, "Line too long. Exceeds buffer size\n");
                            exit(EXIT_FAILURE);
                        }
                        buffer[index] = *ch; 
                        index++;
                        *ch = fgetc(fptr); 
                    }
                    lineNumber++;

                    //commandIndex is the index of a token(word in command) within a command line
                    commandIndex = 0;
                    
                    //Tokenize the line and add the command and each argument to the array
                    commandToken = strtok(buffer, " ");
                    while(commandToken != NULL)
                    {
                        //A new token of a command is found
                        //Allocate space for token and set data
                        char* tempName = malloc(SIZE * sizeof(char));
                        strcpy(tempName, commandToken);

                        //Add the command token to the list of command tokens
                        currTarget->commands[commandLineIndex][commandIndex] = tempName; 
                        commandIndex++;
                        commandToken = strtok(NULL, " ");
                    }

                    //provide a NULL character for the execvp command
                    currTarget->commands[commandLineIndex][commandIndex + 1] = NULL;
                    commandLineIndex++;

                    //We have iterated through each character of a line(it was a command line)
                    startLine = 1;
                   
                }
                else if(isalpha(*ch) || isdigit(*ch))
                {
                    //We have hit a new target. Allocate space for a new target and link it to the list of targets
                    targetNode* temp = malloc(sizeof(targetNode));
                    temp->firstDependency = malloc(sizeof(dependencyNode));
                    temp->nextTarget = NULL;
                    temp->targetName = malloc(SIZE * sizeof(char));
                    currTarget->nextTarget = temp;
                    currDependency = temp->firstDependency;
                    temp->commands = malloc(SIZE * sizeof(char**));
                    //commandIndex = 0;
                    commandLineIndex = 0;
                    currTarget = temp;
                    
                    //Get the name of the target(Everything before ':' character)
                    index = 0;
                    while((*ch != ' ') && (*ch != ':'))
                    {
                        currTarget->targetName[index] = *ch;
                        *ch = fgetc(fptr);
                        index++;

                        if(index > BUFF_SIZE)
                        {
                            fprintf(stderr, "Line too long. Exceeds buffer size\n");
                            exit(EXIT_FAILURE);
                        }
                    }

                    if(*ch == ' ')
                    {
                        while(*ch != ':')
                        {
                            *ch = fgetc(fptr);
                        }   
                    }

                    

                    //Get dependency names(Everything after ':' character)
                    //Put the entire string of dependencies into a buffer
                    index = 0;
                    memset((void*)buffer, 0, BUFF_SIZE);
                    while((*ch != EOF) && (*ch != '\n'))
                    {
                        if(*ch == ':')
                        {
                            buffer[index] = ' ';
                        }
                        else{
                           buffer[index] = *ch; 
                        }
                        index++;
                        *ch = fgetc(fptr); 

                        if(index > BUFF_SIZE)
                        {
                            fprintf(stderr, "Line too long. Exceeds buffer size\n");
                            exit(EXIT_FAILURE);
                        }
                    }
                    lineNumber++;
                    
                    //Tokenize the buffer full of dependency names and add each token to a list of inidividual dependency names
                    dependencyToken = strtok(buffer, " ");
                    while(dependencyToken != NULL)
                    {
                        //A new dependency is found
                        char* tempName = malloc(SIZE * sizeof(char));
                        strcpy(tempName, dependencyToken);
                        currDependency->dependencyName = tempName;

                        tempDependency = malloc(sizeof(dependencyNode));
                        tempDependency->nextDependency = NULL;
                        tempDependency->dependencyName = NULL;

                        currDependency->nextDependency = tempDependency;
                        currDependency = tempDependency;
                        
                        dependencyToken = strtok(NULL, " ");
                    }
                    
                    //We have iterated though an entire target line(a line containing target name and all of its dependencies)
                    startLine = 1;
                }
                else
                {
                    //If a line is not a command or target, then check if it is a comment. If it not a comment then it is an invalid line
                    if(*ch == '#')
                    {
                        //Ignore the entire comment line by iterating through the entire line
                        while((*ch != EOF) && (*ch != '\n'))
                        {
                            *ch = fgetc(fptr);
                        }
                        startLine = 1;
                        lineNumber++;
                    }
                    else
                    {
                        //Invalid Line
                        fprintf(stderr, "%d: Invalid Line: ", lineNumber);
                        while((*ch != EOF) && (*ch != '\n'))
                        {
                            fprintf(stderr, "%s",ch);
                            *ch = fgetc(fptr);
                        }
                        fprintf(stderr, "\n");
                        exit(EXIT_FAILURE);

                    }
                }
            }
		}
    }

    free(ch);
    free(buffer);
    return headNode;
}