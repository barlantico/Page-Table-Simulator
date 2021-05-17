/*
* Brian Arlantico , 821125494, cssc3403
* Rahul Shivade , 822701861, cssc3453
* CS570 Spring 2021
* Assignment 3, pagetable
* main.hpp
*/
#define MAX_BITS 32
#define FIRST_LEVEL 0 
#define ADDRESS_SPACE 0xFFFFFFFF

/*
    Given a logical address, translates it to a page
*/
unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);

/*
    If the offset output is chosen, used to do computations and print results
*/
void offsetFlagFunction(PageTable *pgTabPtr, FILE *fp, int numAddr, bool nflag);
void offsetFlagHelper(FILE *fp, unsigned int offsetMask, p2AddrTr trace_item);
/*
    If no output is specified or summary specified, then call this function for summary output
*/
void summaryFunction(PageTable *pgTabPtr, FILE *fp, int numAddr, bool nflag);
void summaryFunctionHelper(PageTable *pgTabPtr, FILE *fp, unsigned int &addressCounter, unsigned int &hits, unsigned int &misses, unsigned int &frames, p2AddrTr trace_item);

/*
    Function for page2frame output flag
*/
void page2FrameFunction(PageTable *pgTabPtr, FILE *fp, int numAddr, bool nflag);
void page2FrameHelper(PageTable *pgTabPtr, FILE *fp, unsigned int &frameNumber, unsigned int *pages, p2AddrTr trace_item);
/*
    Function for logical2physical flag
*/
void logical2physcialFunction(PageTable *pgTabPtr, FILE *fp, int numAddr, bool nflag);
void logical2physicalHelper(PageTable *pgTabPtr, FILE *fp, unsigned int &frameNumber, p2AddrTr trace_item);