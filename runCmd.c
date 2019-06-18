/*/////////////////////////////
CS 537 Prog 3
Authors: Peyton Gardipee
         Tushar Verma
File: runCMD.c      
*///////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include "fileParser.h"
#include "graphNode.h"
#include "constants.h"
#include "runCmd.h"
#include "outOfDate.h"


targetNode* runCmd(targetNode *headTarget)
{  
    targetList *currDependentTarget;
    targetNode* currTarget;

    //Base Case: If there are no children, return 0;
    if(headTarget->dependentTargets->nextTarget == NULL)
    {
        
        currTarget = headTarget;
        execute(currTarget);
        return headTarget;
    }
    else
    {    
        currDependentTarget = headTarget->dependentTargets->nextTarget;

        while(currDependentTarget != NULL)
        {
            currTarget = runCmd(currDependentTarget->target);

            execute(currTarget);
            currDependentTarget = currDependentTarget->nextTarget;
        }

        currTarget = headTarget;
        execute(currTarget);
    }   
    return headTarget;
}


//Function used to execute commands of a given target
void execute(targetNode *currTarget)
{
    //Variables used by fork, execvp, and wait
    char** args;
    pid_t child;
    int cstatus, index;
    //index = 0;

    if(outOfDate(currTarget))
    {


        if(!currTarget->hasVisited)
        {
            index = 0;
            while(currTarget->commands[index] != NULL)
            {
                args = currTarget->commands[index];
                if ((child = fork()) == 0)
                {
                    //We are in the child
                    execvp(args[0], args);
                    exit(1);
                }
                else
                { // Parent process
                    if (child == (pid_t)(-1))
                    {
                        fprintf(stderr, "Fork failed.\n");
                        exit(1);
                    }
                    else
                    {
                        //Wait for child to complete
                        wait(&cstatus);
                    }
                }
                index++;
            }
        }
    }

    currTarget->hasVisited = 1;
}











