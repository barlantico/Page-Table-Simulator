/*
* Brian Arlantico , 821125494, cssc3403
* Rahul Shivade , 822701861, cssc3453
* CS570 Spring 2021
* Assignment 3, pagetable
* Map.hpp
*/
class Map {

    public: 
        bool valid; 
        bool frameAssigned; //reveals if frame has been assigned a frame
        unsigned int frame;

    Map();
    ~Map();
};