# File: Makefile
# CS 375
# Project 2
# Date Due: Sept. 20, 2018

all: kitten lsl count-opens 

count-opens: count-opens.cpp
	g++ -g -Wall -o count-opens count-opens.cpp -std=c++11
lsl: lsl.cpp
	g++ -g -Wall -o lsl lsl.cpp -std=c++11
kitten: kitten.cpp
	g++ -g -Wall -o kitten kitten.cpp -std=c++11
clean:
	rm -f *.o count-opens lsl kitten *.*~
