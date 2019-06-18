/*/////////////////////////////
CS 537 Prog 3
Authors: Peyton Gardipee
         Tushar Verma
File: outOfDate.c      
*/////////////////////////////

#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include<stdlib.h>
#include <stdio.h>
#include "graphNode.h"

int outOfDate(targetNode *target)
{
    struct stat targetTime;
    struct stat currDependencyTime;

    dependencyNode *currDependency;

    currDependency = target->firstDependency;

    int outOfDate = 0;

    FILE *fptr;
    fptr = fopen(target->targetName, "r");

    //If the target is not a file(ie. "all") then always make that target
    if(fptr == NULL)
    {
        return 1;
    }

    //Attempt to get target's stat time values
    if(stat(target->targetName,&targetTime)== -1) 
    {
        fprintf(stderr,"stat failed");
        exit(EXIT_FAILURE);
    }

    //If the target has no dependencies, then always run that target's commands
    if(currDependency == NULL)
    {
        return 1;
    }
    else{
        while(currDependency->dependencyName != NULL)
        {

            fptr = fopen(currDependency->dependencyName, "r");
            //If the dependency is not a file(ie. "all") then always run that target's commands
            if(fptr == NULL)
            {
                return 1;
            }

            if(stat(currDependency->dependencyName, &currDependencyTime)== -1)
            {
                fprintf(stderr,"stat failed");
                exit(EXIT_FAILURE);
            }


            if(difftime(currDependencyTime.st_mtime, targetTime.st_mtime) > 0)
            {
                //A dependency is more up to date than the target. The target is out of date
                outOfDate = 1;
            }

            currDependency = currDependency->nextDependency;
        }
    }

    return outOfDate;
}