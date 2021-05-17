/*
* Brian Arlantico , 821125494, cssc3403
* Rahul Shivade , 822701861, cssc3453
* CS570 Spring 2021
* Assignment 3, pagetable
* Level.hpp
*/
#include "PageTable.hpp" //required for forward reference
class Level {

    public:
        unsigned int depth;
        PageTable *pageTabPtr; //pointer to PageTable object
        Level **NextLevelPtr;
        Map *mapPtr;

    Level(int depth, PageTable *pageTabPtr);

    ~Level();
    //finds size allocated by current level
    unsigned int getLevelSize();
};