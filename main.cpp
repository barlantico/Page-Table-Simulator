/*
* Brian Arlantico , 821125494, cssc3403
* Rahul Shivade , 822701861, cssc3453
* CS570 Spring 2021
* Assignment 3, pagetable
* main.cpp
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <cmath>

#include "output_mode_helpers.h"
#include "byutr.h"
#include "Level.hpp"
#include "main.hpp"

int main(int argc, char **argv)
{
    //declarations                                                                            
    int c, index; 
    bool bitmaskflag = 0, log2physflag = 0, p2fflag = 0, offsetflag = 0, sumflag = 0, nflag = 0;
    
    unsigned int numOfAddr = 0; 
    while ((c = getopt(argc, argv, "n:o:")) != -1) //checks arguments 
    {
        switch(c)
        {
            case 'n': //num of addresses to process
                numOfAddr = (unsigned int) atoi(optarg);
                nflag = 1;
                break;
            case 'o':
                if (strcmp("bitmasks", optarg) == 0)
                {
                    bitmaskflag = 1;
                    break;
                }
                else if (strcmp("offset", optarg) == 0)
                {
                    offsetflag = 1;
                    break;
                }
                else if (strcmp("logical2physical", optarg) == 0)
                {
                    log2physflag = 1;
                    break;
                }
                else if (strcmp("page2frame", optarg) == 0)
                {
                    p2fflag = 1;
                    break;
                }
                else if (strcmp("summary", optarg) == 0)
                {
                    sumflag = 1;
                    break;
                }
                else //if no output mode recognized
                    std::cout << "Unknown output mode" << std::endl;
            case '?':
                std::cout << "Usage: pagetable (-n int) (-o bitmasks/logical2physical/page2frame/offset/summary) <tracefile> [0-9* ]*" << std::endl;
                exit(0); //exits if unknown flag
        }
    }
    //OPEN FILE AND WORK WITH ARGUMENTS 
    index = optind;

    FILE *fp = fopen(argv[index++], "r");
    if (fp == NULL)
    {
        std::cout << "Error: Unable to open file" << std::endl;
        exit(1);
    }
    //creates PageTable object and initializes first level
    PageTable *pageTab = new PageTable(argc, argv, index);
    pageTab->rootPtr = new Level(FIRST_LEVEL, pageTab);

    //case statements for each output mode (summary is default output mode if not specified)
    if (offsetflag)
        offsetFlagFunction(pageTab, fp, numOfAddr, nflag);
    else if (bitmaskflag)
        report_bitmasks(pageTab->LevelCount, pageTab->BitmaskAry);
    else if (p2fflag)
        page2FrameFunction(pageTab, fp, numOfAddr, nflag);
    else if (log2physflag)
        logical2physcialFunction(pageTab, fp, numOfAddr, nflag);
    else //default argument
        summaryFunction(pageTab, fp, numOfAddr, nflag);

    delete pageTab;

    return 0;
}

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift) 
{   //performs mask and shift to return page number
    unsigned int pageNum = LogicalAddress;
    pageNum = pageNum & Mask;
    pageNum = pageNum >> Shift;
    return pageNum;
}

void offsetFlagFunction(PageTable *pgTabPtr, FILE *fp, int numAddr, bool nflag)
{
    p2AddrTr trace_item; //struct with trace information
    //compute offfsetMask for pagetable 
    unsigned int offsetMask = ADDRESS_SPACE >> (MAX_BITS - pgTabPtr->ShiftAry[pgTabPtr->LevelCount - 1]);
    bool done = false;
    if (nflag) 
        for (int i = 0; i < numAddr; i++)
        {
            int bytesread = NextAddress(fp, &trace_item);
            done = bytesread == 0;
            if (!done)
                offsetFlagHelper(fp, offsetMask, trace_item);
            else 
                break;
        }
    else
    {
      while (!done)
      {
        int bytesread = NextAddress(fp, &trace_item);
        done = bytesread == 0;
        if (!done)
          offsetFlagHelper(fp, offsetMask, trace_item);
        } 
    }
}

void offsetFlagHelper(FILE *fp, unsigned int offsetMask, p2AddrTr trace_item)
{
    //performs bitwise and and prints out offset information
    unsigned int logAddr = trace_item.addr;
    unsigned int offsetAddr = logAddr & offsetMask;
    report_logical2offset(logAddr, offsetAddr);
}

void summaryFunction(PageTable *pgTabPtr, FILE *fp, int numAddr, bool nflag)
{
    p2AddrTr trace_item;  /* Structure with trace information */
    //initialize variables to be args for report_summary function
    unsigned int frameNumber = 0;
    unsigned int totalBytes = 0;
    unsigned int hits = 0; unsigned int misses = 0; unsigned int addressCounter = 0;
    unsigned int pageSize = pow(2,pgTabPtr->ShiftAry[pgTabPtr->LevelCount - 1]);
    bool done = false;
    
    //insert addresses into pagetable
    if (nflag)
    {
        for (int i = 0; i < numAddr; i++) 
        {
            int bytesread = NextAddress(fp, &trace_item);
            done = bytesread == 0;
            if (!done)
                summaryFunctionHelper(pgTabPtr, fp, addressCounter, hits, misses, frameNumber, trace_item);
            else
                break;
        }
    }
    else
    {
        while (!done)
        {
            int bytesread = NextAddress(fp, &trace_item);
            done = bytesread == 0;
            if (!done)
                summaryFunctionHelper(pgTabPtr, fp, addressCounter, hits, misses, frameNumber, trace_item);
        }
    }
    //calculate total bytes allocated by objects
    totalBytes += pgTabPtr->getPageTableSize();
    totalBytes += pgTabPtr->rootPtr->getLevelSize();
    report_summary(pageSize, hits, addressCounter, frameNumber, totalBytes);

}

//counts number of addresses processed and determines hits/misses
void summaryFunctionHelper(PageTable *pgTabPtr, FILE *fp, unsigned int &addressCounter, unsigned int &hits, unsigned int &misses, unsigned int &frames, p2AddrTr trace_item)
{
    if (pgTabPtr->PageLookup(trace_item.addr) == NULL)
    {
        pgTabPtr->PageInsert(trace_item.addr, frames);
        misses++;
        frames++;
    }
    else
        hits++;
    
    addressCounter++;
}

void page2FrameFunction(PageTable *pgTabPtr, FILE *fp, int numAddr, bool nflag)
{
  p2AddrTr trace_item;  /* Structure with trace information */
  unsigned int frameNumber = 0;
  unsigned int pages[pgTabPtr->LevelCount]; //used as page number associated with level
  bool done = false;

  //insert addresses into page table and call helper function
  if (nflag)
    for (int i = 0; i < numAddr; i++) 
    { 
      int bytesread = NextAddress(fp, &trace_item);
      done = bytesread == 0;
      if (!done)
        page2FrameHelper(pgTabPtr, fp, frameNumber, pages, trace_item);
      else
        break;
    }
    else 
    {
      while (!done)
      {
        int bytesread = NextAddress(fp, &trace_item);
        done = bytesread == 0;
        if (!done)
          page2FrameHelper(pgTabPtr, fp, frameNumber, pages, trace_item);
        }
    }
}

void page2FrameHelper(PageTable *pgTabPtr, FILE *fp, unsigned int &frameNumber, unsigned int *pages, p2AddrTr trace_item)
{
    // Increments frame number if page does not exist
    if (pgTabPtr->PageLookup(trace_item.addr) == NULL)
    {
        pgTabPtr->PageInsert(trace_item.addr, frameNumber);
        frameNumber++;
    }
    //calculate page number for each page level
    for (int j = 0; j < pgTabPtr->LevelCount; j++)
        pages[j] = LogicalToPage(trace_item.addr, pgTabPtr->BitmaskAry[j], pgTabPtr->ShiftAry[j]);

    //find frame number of the page
    unsigned int temp = LogicalToPage(trace_item.addr, pgTabPtr->BitmaskAry[pgTabPtr->LevelCount - 1], pgTabPtr->ShiftAry[pgTabPtr->LevelCount - 1]);
    unsigned int tempFrame = pgTabPtr->PageLookup(trace_item.addr)[temp].frame;
    report_pagemap(trace_item.addr, pgTabPtr->LevelCount, pages, tempFrame);

}

void logical2physcialFunction(PageTable *pgTabPtr, FILE *fp, int numAddr, bool nflag)
{
    p2AddrTr trace_item;
    unsigned int frameNumber = 0;
    bool done = false;

    //insert address into page table and call logical2physical helper funciton
    if (nflag)
        for (int i = 0; i < numAddr; i++)
        { 
            int bytesread = NextAddress(fp, &trace_item);  
            done = bytesread == 0;
            if (!done)
                logical2physicalHelper(pgTabPtr, fp, frameNumber, trace_item);
            else
                break;
        }
    else
    {
        while (!done)
        {
          int bytesread = NextAddress(fp, &trace_item);
          done = bytesread == 0;
          if (!done)
            logical2physicalHelper(pgTabPtr, fp, frameNumber, trace_item);
        }
    }

} 

void logical2physicalHelper(PageTable *pgTabPtr, FILE *fp, unsigned int &frameNumber, p2AddrTr trace_item)
{
    //inserts if not found in pagetable
    if (pgTabPtr->PageLookup(trace_item.addr) == NULL)
    {
        pgTabPtr->PageInsert(trace_item.addr, frameNumber);
        frameNumber++;

    }

    //calculates physical adress through (physical address = frameNumber * pagesize + offset)
    unsigned int pageBits = pgTabPtr->ShiftAry[pgTabPtr->LevelCount - 1];
    unsigned int pageSize = pow(2,pageBits);
    unsigned int temp = LogicalToPage(trace_item.addr, pgTabPtr->BitmaskAry[pgTabPtr->LevelCount - 1], pgTabPtr->ShiftAry[pgTabPtr->LevelCount - 1]);
    unsigned int tempFrame = pgTabPtr->PageLookup(trace_item.addr)[temp].frame;
    pageSize *= tempFrame;
    unsigned int offsetMask = ADDRESS_SPACE >> (MAX_BITS - pgTabPtr->ShiftAry[pgTabPtr->LevelCount - 1]);
    unsigned int offsetTemp = trace_item.addr & offsetMask;

    pageSize += offsetTemp;
    report_logical2physical(trace_item.addr, pageSize);
}
