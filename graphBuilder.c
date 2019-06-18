/*/////////////////////////////
CS 537 Prog 3
Authors: Peyton Gardipee
         Tushar Verma
File: graphBuilder.c      
*///////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileParser.h"
#include "graphNode.h"
#include "constants.h"


targetNode* graphBuilder(targetNode *headTarget)
{
    targetNode *currTargetInner, *currTargetOuter;
    dependencyNode *currDependency;
    targetList *currDependentTarget, *tempDependentTarget;

    //Start each loop at the beginning of the list of targets
    currTargetOuter = headTarget->nextTarget;

    //Iterate through each of the target nodes
    while(currTargetOuter != NULL)
    {
        //Iterate through each of the current target's dependencies
        currDependency = currTargetOuter->firstDependency;
        currTargetOuter->dependentTargets = malloc(sizeof(targetList));
        currDependentTarget = currTargetOuter->dependentTargets;
        while(currDependency->dependencyName != NULL)
        {
            //Iterate through each of the target nodes again to add each target with the name of the current dependency
            //to the current target's list of dependent nodes
            currTargetInner = headTarget->nextTarget;
            while(currTargetInner != NULL)
            {   
                if(!strcmp(currDependency->dependencyName,currTargetInner->targetName))
                {
                    tempDependentTarget = malloc(sizeof(targetList));
                    tempDependentTarget->target = currTargetInner;
                    tempDependentTarget->nextTarget = NULL;

                    currDependentTarget->nextTarget = tempDependentTarget;
                    currDependentTarget = tempDependentTarget;
                }
                currTargetInner = currTargetInner->nextTarget;
            }
            currDependency = currDependency->nextDependency;
        }
        currTargetOuter = currTargetOuter->nextTarget;
    }

    return headTarget;
}


//Function used to get a target node given its target name
targetNode* getNode(char* targetName, targetNode* headTarget)
{
    targetNode *currTarget;

    currTarget = headTarget->nextTarget;

    //Iterate through each target to find a match according to name
    while(currTarget != NULL)
    {
        //Return target if match is found
        if(!strcmp(currTarget->targetName, targetName))
        {
            return currTarget;
        }
        currTarget = currTarget->nextTarget;
    }

    //No target was found with given name
    return NULL;
}


