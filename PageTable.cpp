/*
* Brian Arlantico , 821125494, cssc3403
* Rahul Shivade , 822701861, cssc3453
* CS570 Spring 2021
* Assignment 3, pagetable
* PageTable.cpp
*/
#define MAX_BITS 32
#define MIN_BITS_FOR_PAGE 1

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>

#include "Level.hpp"

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);
PageTable::PageTable(int argc, char **argv, int index)
{
    LevelCount = argc - index;
    if (LevelCount == 0)
    {
        std::cout << "Error: Unspecified bits per level. Please specify bits per level with a sum less than or equal to 31 bits" << std::endl;
        exit(0);
    }
    //initializes each of the arrays after solving for level count
    BitmaskAry = (unsigned int*) malloc(LevelCount * sizeof(unsigned int));
    ShiftAry = (unsigned int*) malloc(LevelCount * sizeof(unsigned int));
    EntryCount = (unsigned int*) malloc(LevelCount * sizeof(unsigned int));
    int temp = MAX_BITS;
    unsigned int maskTemp = 0; //temp used to solve for each bitmaskary element

    for (int i = 0; i < LevelCount; i++)
    {
        EntryCount[i] = (unsigned int) pow(2,atoi(argv[index])); //power of 2 to find number of entries in a level
        temp -= atoi(argv[index++]); //calculates amount of bits to shift for each level
        ShiftAry[i] = (unsigned int) temp; 
        maskTemp = EntryCount[i] - 1; //creates 1 bits for amount of entries in that level
        BitmaskAry[i] = maskTemp << temp; //shift to appropriate position
    }

    if (temp < MIN_BITS_FOR_PAGE)
    {
        std::cout << "Error: Minimum of 1 bit required for page size" << std::endl;
        exit(0);
    }
}

PageTable::~PageTable()
{   //frees all memory allocated by PageTable object
    free(BitmaskAry);
    free(ShiftAry);
    free(EntryCount);
    free(rootPtr);

}

Map* PageTable::PageLookup(unsigned int LogcialAddress)
{   
    Level *lvlptr; //level pointer used to traverse tree
    lvlptr = rootPtr;
    int i = 0;
    //retrieve virtual page number
    unsigned int temp;
    for (i = 0; i < LevelCount - 1; i++)
    {
        temp = LogicalToPage(LogcialAddress, BitmaskAry[i],ShiftAry[i]);
        if (lvlptr->NextLevelPtr[temp] == NULL)
            return NULL; //if no level allocated then page is not mapped
        else
            lvlptr = lvlptr->NextLevelPtr[temp]; //continue
    }
    //last level, check the map array
    temp = LogicalToPage(LogcialAddress, BitmaskAry[i],ShiftAry[i]);
    if (lvlptr->mapPtr[temp].valid == 1)
    {
        return lvlptr->mapPtr; //map is valid so page is mapped
    }
    else
        return NULL; //page is not mapped, but structures exist
}


void PageTable::PageInsert(unsigned int LogicalAddress, unsigned int Frame)
{
    Level *lvlptr; //level pointer to traverse tree
    lvlptr = rootPtr;
    int i = 0;
    unsigned int temp;
    for (i = 0; i < LevelCount - 1; i++)
    {
         temp = LogicalToPage(LogicalAddress, BitmaskAry[i],ShiftAry[i]);
         if (lvlptr->NextLevelPtr[temp] == NULL)
         {  //initalize new level if one has not already been 
            lvlptr->NextLevelPtr[temp] = new Level(i + 1, this);
            lvlptr = lvlptr->NextLevelPtr[temp];
         }
        else //traverse tree
            lvlptr = lvlptr->NextLevelPtr[temp];
    }
    //calculates page number of map element 
    temp = LogicalToPage(LogicalAddress, BitmaskAry[i],ShiftAry[i]);

    //assigns frame if frame encountered for the first time
    if (!lvlptr->mapPtr[temp].frameAssigned)
    {
        lvlptr->mapPtr[temp].frame = Frame;
        lvlptr->mapPtr[temp].frameAssigned = true;
    }
    lvlptr->mapPtr[temp].valid = 1; //validates map

}

unsigned int PageTable::getPageTableSize() 
{
    unsigned int bytes = 0;
    bytes = sizeof(EntryCount) + sizeof(BitmaskAry) + sizeof(ShiftAry);
    bytes *= LevelCount;
    bytes += sizeof(rootPtr) + sizeof(LevelCount);
    //adds all the sizes of the variables of PageTable object
    return bytes;
}