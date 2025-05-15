CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
SOURCES = main.cpp parser.cpp vm.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = jit_compiler

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)