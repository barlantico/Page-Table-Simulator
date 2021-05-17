/*
* Brian Arlantico , 821125494, cssc3403
* Rahul Shivade , 822701861, cssc3453
* CS570 Spring 2021
* Assignment 3, pagetable
* Level.cpp
*/
#include <stdlib.h>

#include "Level.hpp"

Level::Level(int depth, PageTable *pgTabPtr)
{
    this->depth = depth;
    pageTabPtr = pgTabPtr;

    if (depth != pgTabPtr->LevelCount - 1)
    {
        NextLevelPtr = (Level**) malloc(sizeof(Level*) * pgTabPtr->EntryCount[depth]);
        //allocate nextlevel if current level is not the last level
    }
    else
    {
        mapPtr = new Map[pgTabPtr->EntryCount[depth]]; //allocate maps if current level is last level
    }

}

Level::~Level() //destructor checks which case to clear memory 
{
        if (depth == pageTabPtr->LevelCount - 1)
            delete[] NextLevelPtr;
        else
            delete mapPtr;

        free(pageTabPtr);
}

unsigned int Level::getLevelSize() 
{
    unsigned int bytes;
    //if leaf node count maps, else count levels
    if (depth == pageTabPtr->LevelCount - 1)
    {
       bytes += sizeof(Map) * pageTabPtr->EntryCount[depth];
    }
    else
    {
        for (int i = 0; i < pageTabPtr->EntryCount[depth]; i++)
        {
            if (NextLevelPtr[i] == NULL)
                bytes += sizeof(NULL); //adds null pointer size
            else
                bytes += NextLevelPtr[i]->getLevelSize(); //recursive call
        }
    }
    bytes += sizeof(pageTabPtr);
    bytes += sizeof(depth);
    return bytes;
}
