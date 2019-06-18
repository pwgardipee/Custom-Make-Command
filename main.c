/*////////////////////////////
CS 537 Prog 3
Authors: Peyton Gardipee
         Tushar Verma
File: main.c        
*////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include "constants.h"
#include "fileParser.h"
#include "graphNode.h"
#include "runCmd.h"
#include "outOfDate.h"
#include "detectCycle.h"


int main(int argc, char* argv[])
{
    char* targetName;
    int opt;

    targetNode *headTarget, *currTarget, *tempTarget;



    switch(argc)
    {
        case 1:
            //Parse the file and store the node corresponding to the first target of makefile
            headTarget = fileParser(NULL);

            //Build the graph
            graphBuilder(headTarget);

            //Check for cycles in whole graph
            currTarget = headTarget->nextTarget;
            while(currTarget != NULL)
            {

                tempTarget = headTarget->nextTarget;

                while(tempTarget != NULL)
                {
                    tempTarget->hasVisited = 0;
                    tempTarget = tempTarget->nextTarget;
                }

                detectCycle(currTarget);
                currTarget = currTarget->nextTarget;
            }

            currTarget = headTarget->nextTarget;

            while(currTarget != NULL)
            {
                currTarget->hasVisited = 0;
                currTarget = currTarget->nextTarget;
            }

            currTarget = headTarget->nextTarget;

            runCmd(currTarget);

            break;
        case 2:
            //Get the name of the specific target to run
            targetName = argv[1]; 

            //Run standard file parsing and graph builder on file
            headTarget = fileParser(NULL);
            graphBuilder(headTarget);

            //Check for cycles in whole graph
            currTarget = headTarget->nextTarget;
            while(currTarget != NULL)
            {
                tempTarget = headTarget->nextTarget;

                while(tempTarget != NULL)
                {
                    tempTarget->hasVisited = 0;
                    tempTarget = tempTarget->nextTarget;
                }

                detectCycle(currTarget);
                currTarget = currTarget->nextTarget;
            }

            //Get the target corresponding to the desired target name
            currTarget = getNode(targetName, headTarget);
            
            //If the specified target exists, run its commands
            if(currTarget == NULL)
            {
                fprintf(stderr, "Specified target does not exist.\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                runCmd(currTarget);
            }

            break;
        case 3:
            if (( opt = getopt(argc,argv,"f:")) != -1 ) {
                //Parse the file and store the node corresponding to the first target of makefile
                headTarget = fileParser(optarg);

                //Build the graph
                graphBuilder(headTarget);

                //Check for cycles in whole graph
                currTarget = headTarget->nextTarget;
                while(currTarget != NULL)
                {
                    tempTarget = headTarget->nextTarget;

                    while(tempTarget != NULL)
                    {
                        tempTarget->hasVisited = 0;
                        tempTarget = tempTarget->nextTarget;
                    }

                    detectCycle(currTarget);
                    currTarget = currTarget->nextTarget;
                }

                currTarget = headTarget->nextTarget;

                while(currTarget != NULL)
                {
                    currTarget->hasVisited = 0;
                    currTarget = currTarget->nextTarget;
                }

                currTarget = headTarget->nextTarget;

                runCmd(currTarget);
            }
            else
            {
                fprintf(stderr,"Invalid command line entry.\n");
                exit(EXIT_FAILURE);
            }

            break;
        case 4:
            if (( opt = getopt(argc,argv,"f:")) != -1 ) {
                //Get the name of the specific target to run
                targetName = argv[3]; 

                //Run standard file parsing and graph builder on file
                headTarget = fileParser(optarg);
                graphBuilder(headTarget);

                //Check for cycles in whole graph
                currTarget = headTarget->nextTarget;
                while(currTarget != NULL)
                {
                    tempTarget = headTarget->nextTarget;

                    while(tempTarget != NULL)
                    {
                        tempTarget->hasVisited = 0;
                        tempTarget = tempTarget->nextTarget;
                    }

                    detectCycle(currTarget);
                    currTarget = currTarget->nextTarget;
                }

                //Get the target corresponding to the desired target name
                currTarget = getNode(targetName, headTarget);
                
                //If the specified target exists, run its commands
                if(currTarget == NULL)
                {
                    fprintf(stderr, "Specified target does not exist.\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    runCmd(currTarget);
                }

            }
            else
            {
                fprintf(stderr,"Invalid command line entry.\n");
                exit(EXIT_FAILURE);
            }

            break;
        default:
            fprintf(stderr, "Invalid input.\n");
    }
}