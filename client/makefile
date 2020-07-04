.PHONY: all build test clean

SRC_FILES := $(wildcard *.cpp) $(wildcard pattern)
BINS := $(SRC_FILES:%.cpp=%)

# Output File:
TARGET_EXTENSION := .out
TARGET_BASE := client_burn-in-app
TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)

# Compiler Setup
CC := g++
LDFLAGS := -lm -lpthread -pthread
CFLAGS=-std=c++11

all: build test
 
build:
	
	@echo "Going to Compile now!"
	$(CC) $(LDFLAGS) $(CFLAGS) $(SRC_FILES) -o $(TARGET)


test:
	@echo "No tests are defined!"
	#./$(TARGET)
						
clean:	
	@echo "Cleaning up..."
	rm $(TARGET)
	@echo "Done" 
