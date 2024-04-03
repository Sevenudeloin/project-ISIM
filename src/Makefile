CXX ?= g++
# Can remove some flags if needed (like Werror)
CXXFLAGS= -std=c++20 -Wall -Wextra -Werror -pedantic \
		  -Wold-style-cast

# TODO add all the files that need to be compiled
OBJS = interval.o color.o diamond-square.o image2d.o main.o pixel.o vector3.o ray.o terrain.o heightmap.o physobj.o triangle.o camera.o scene.o light.o ppm_parser.o rendering.o skybox.o
all: proc_gen

proc_gen: $(OBJS)
	$(CXX) -o $@ $^

clean:
	$(RM) $(OBJS) proc_gen
.PHONY:
	clean

run: proc_gen
	./proc_gen