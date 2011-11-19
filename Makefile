
SOURCES=mergex.cpp
EXECUTABLE=mergex

CC=g++
CFLAGS=-c -Wall -DMARKUP_STDCONV
LDFLAGS=-static-libgcc -static-libstdc++ -Os -s -fno-rtti -fno-exceptions
OBJECTS=$(SOURCES:%.cpp=%.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all clean

