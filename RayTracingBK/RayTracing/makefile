CC=clang++
CFLAGS=-c -O3 -std=c++11
LDFLAGS=-lglut -lGL -lGLU
SOURCES=ray_tracing.cpp Sphere.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
