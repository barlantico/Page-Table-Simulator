/*
* Brian Arlantico , 821125494, cssc3403
* Rahul Shivade , 822701861, cssc3453
* CS570 Spring 2021
* Assignment 3, pagetable
* PageTable.hpp
*/
#include "Map.hpp"
class Level;
class PageTable {
    public:
        unsigned int LevelCount; //number of levels in the page table
        unsigned int *BitmaskAry; //bitmasks for corresponding levels
        unsigned int *ShiftAry; //amount of bits to shift
        unsigned int *EntryCount; //number of entries for each level
        Level *rootPtr; //pointer to root level

        PageTable(int argc, char **argv, int index);

        ~PageTable();

        //Given a logical address, returns map object if valid, otherwise return NULL
        Map *PageLookup(unsigned int LogcialAddress);

        //Given a logical address, inserts new entry into page table
        void PageInsert(unsigned int LogicalAddress, unsigned int Frame);

        //returns size of pagetable object in bytes
        unsigned int getPageTableSize();
};

