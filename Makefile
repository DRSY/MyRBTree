CC=g++
C_FLAGS=-std=c++17
SOURCE=main.cpp

all: 
    $(CC) $(C_FLAGS) $(SOURCE)

clean:
    rm a.out
