/*
* Brian Arlantico , 821125494, cssc3403
* Rahul Shivade , 822701861, cssc3453
* CS570 Spring 2021
* Assignment 3, pagetable
* Map.cpp
*/
#include "Map.hpp"

Map::Map()
{
    valid = false; //all new map objects begin with no mapping to frame
    frameAssigned = 0;
    frame = 0;
}

Map::~Map()
{
    
}