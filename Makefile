# Common definitions
CXX = g++

# Compiler flags, paths and libraries
CFLAGS = -pedantic -Wall -Wextra -O3 -fopenmp
LFLAGS = $(CFLAGS)
LIBS   =

TGTS = c1
OBJS = c1.cpp

all: c1

c1: $(OBJS) Makefile random.h material.h camera.h hitable_list.h hitable.h ray.h sphere.h vec3.h
	$(CXX) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

run: c1
	./c1 > test.ppm && feh test.ppm

clean:
	$(RM) $(TGTS)

default: c1
