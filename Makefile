#Compile
CC = g++
FLAGS = -Wall -g

#Source folder

SRC = $(realpath $(CURDIR))/src/

all:PalindromeMasked

PalindromeMasked: Palindrome_masked_seed.o command_line_parser.o
		$(CC) $(FLAGS) $(INC) -o PalindromeMasked Palindrome_masked_seed.o command_line_parser.o

Palindrome_masked_seed.o: $(SRC)Palindrome_masked_seed.cpp
		$(CC) $(FLAGS) $(INC) -c $(SRC)Palindrome_masked_seed.cpp
command_line_parser.o: $(SRC)command_line_parser.cpp $(SRC)command_line_parser.hpp
		$(CC) $(FLAGS) $(INC) -c $(SRC)command_line_parser.cpp
clean:
	rm -f mask_mers.o command_line_parser.o PalindromeMasked