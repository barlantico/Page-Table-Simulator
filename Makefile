###########################################################
# Brian Arlantico , 821125494, cssc3403
# Rahul Shivade , 822701861, cssc3453
# CS570 Spring 2021
# Assignment 3, pagetable
# Makefile
###########################################################
CXX = g++
prog = pagetable
CXXFLAGS=-std=c++11 -g

$(prog): main.o PageTable.o Level.o Map.o
	$(CXX) $(CXXFLAGS) -o $(prog) main.o PageTable.o Level.o Map.o byu_tracereader.c output_mode_helpers.c

main.o: main.cpp Level.hpp byutr.h output_mode_helpers.h main.hpp
	$(CXX) -c main.cpp

PageTable.o: PageTable.cpp Level.hpp
	$(CXX) -c PageTable.cpp

Level.o: Level.cpp Level.hpp
	$(CXX) -c Level.cpp

Map.o: Map.cpp Map.hpp
	$(CXX) -c Map.cpp

clean:
	rm $(prog) *.o 