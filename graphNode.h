#ifndef GRPAHNODE_H
#define GRAPHNODE_H

struct targetNode* graphBuilder( struct targetNode *headTarget);

struct targetNode* getNode(char* targetName, struct targetNode* headTarget);

//The graph will be made up of targetNode structs
typedef struct targetNode{
    struct targetNode* nextTarget;
    struct dependencyNode* firstDependency;
    char*** commands;
    //struct dependencyNode* dependentTargets;
    struct targetList* dependentTargets;
    char* targetName;
    int hasVisited;
    int outDated;
}targetNode;

//Used to store dependencies of a target
typedef struct dependencyNode{
    struct dependencyNode* nextDependency;
    char* dependencyName;
}dependencyNode;

typedef struct targetList{
    struct targetNode* target;
    struct targetList* nextTarget;
}targetList;

// typedef struct commandNode{
//     struct commandNode* nextCommand;
//     char* commandName;
// }commandNode;


// typedef struct orderNode{
//     struct orderNode* prevNode;
//     struct orderNode* nextNode;
//     char* targetName;
// }orderNode;


#endif