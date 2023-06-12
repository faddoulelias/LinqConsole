.PHONY = all clean run

# Compiler
CXX := g++
CFLAGS := -Wall -g -std=c++17

# Directories
BUILD  := ./build
OBJECT := ./build/obj
SOURCE := ./src
HEADER := ./src/headers

# Files
SRCS := $(wildcard $(SOURCE)/*.cpp)
OBJS := $(patsubst $(SOURCE)/%.cpp, $(OBJECT)/%.o, $(SRCS))
EXEC := $(BUILD)/Server

# Makefile commands
all : $(EXEC)

run:$(EXEC)
	$(EXEC) $(ARGS)

$(EXEC) : $(OBJS)
	$(CXX) $(CFLAGS) -o $(EXEC) $(OBJS)

$(OBJECT)/%.o : $(SOURCE)/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^

clean:
	rm -f $(EXEC) $(OBJS)
