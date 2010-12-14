# Compiler flags
CC=g++
CFLAGS= -I.
# Class name
CLASS=cse484

# Project name
PROJECT=project

# MSU aPID
aPID=riggleza-vanweime

# OBJ Files...
OBJ=$(patsubst %.cpp,%.o,$(wildcard src/*.cpp))

# Output File...
OUTPUT_FILE = cbir

# Source dir
SRC = src

# Include dir
INC = inc
	
all: $(OBJ) cbir 
	

cbir: $(OBJ)
	$(CC) $(SRC)/*.o $(FLAGS) -o $(OUTPUT_FILE) -lboost_program_options -lhdf5 -lboost_filesystem -lboost_system -llog4cxx -lflann

clean:
	rm -f $(SRC)/*.o $(OUTPUT_FILE) $(CLASS)-$(PROJ)-$(aPID)*.tar.gz
	
dist: clean
	tar -cvzf $(CLASS)-$(PROJECT)-$(aPID).tar.gz *
	
dist-final: clean
	astyle --style=kr *.cpp *.h
	rm -f *.orig
	tar -cvzf $(CLASS)-$(PROJ)-$(aPID).final.tar.gz *

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $*.o $*.cpp
