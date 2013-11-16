CC = g++
SRC = src/unit_test.cpp

library:
	$(CC) tashbox_include.h -c $(SRC) --std=c++11 -Wall
	