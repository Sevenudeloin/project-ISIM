CXX ?= g++
# Can remove some flags if needed (like Werror)
CXXFLAGS= -std=c++20 -Wall -Wextra -Werror -pedantic \
		  -Wold-style-cast

# TODO add all the files that need to be compiled
OBJS = interval.o color.o diamond_square.o image2d.o main.o pixel.o vector3.o ray.o \
	terrain.o heightmap.o physobj.o triangle.o camera.o scene.o light.o ppm_parser.o \
	rendering.o skybox.o thread_pool.o simplex_noise.o ocean.o material.o sunlight.o \
	terrain_texture.o ocean_texture.o normal_map_generator.o terrain_texture_map_generator.o \
	terrain_texture_parameters.o absorption_volume.o clouds_plan.o wave_map_generator.o \
	wave_map_parameters.o terrain_layer_texture.o terrain_oceanic_plan.o simplex_island_generator.o \
	dla_generator.o dla_graph.o

all: proc_gen

proc_gen: $(OBJS)
	$(CXX) -o $@ $^

clean:
	$(RM) $(OBJS) proc_gen
.PHONY:
	clean

run: proc_gen
	./proc_gen