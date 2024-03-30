CXX ?= g++
# Can remove some flags if needed (like Werror)
CXXFLAGS= -std=c++20 -Wall -Wextra -Werror -pedantic \
		  -Wold-style-cast

# TODO add all the files that need to be compiled
OBJS = color.o diamond-square.o image2d.o interval.o main.o pixel.o point3.o vector3.o

all: proc_gen

proc_gen: $(OBJS)
	$(CXX) -o $@ $^

clean:
	$(RM) $(OBJS) proc_gen
.PHONY:
	clean

run: proc_gen
	./proc_gen