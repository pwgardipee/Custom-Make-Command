#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fileParser.h"
#include "graphNode.h"
#include "constants.h"


int detectCycle(targetNode *headTarget)
{
    targetList *currDependentTarget;

    //Base Case: If there are no children, return 0;
    if(headTarget->dependentTargets->nextTarget == NULL)
    {
        //No cycle at this point
        return 0;
    }
    else
    {
        //Recursive Case: Check all children for cycles
        if(headTarget->hasVisited)
        {
            fprintf(stderr, "Cycle Detected\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            headTarget->hasVisited = 1;
            currDependentTarget = headTarget->dependentTargets->nextTarget;

            while(currDependentTarget != NULL)
            {
                
                detectCycle(currDependentTarget->target);
                currDependentTarget = currDependentTarget->nextTarget;
            }

            
        }
    }   

    return 0;
}