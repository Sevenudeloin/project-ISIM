# DLA meets Raytracing

Implementation of diffusion-limited aggregation (DLA) for procedural terrain generation and realistic rendering
based on raytracing, in C++.

Project by Oscar Morand and Ewan Lemonnier as part of the final project in an introductory course to image synthesis.

## Final results

DLA algorithm + rendering:
![rendu_final_2](https://github.com/user-attachments/assets/57690187-c571-404b-88d7-d28bd06e7a10)

*TODO add more images and caption accordingly*

## DLA for procedural terrain generation (Ewan Lemonnier)

Algorithm concept by Josh's Channel : ![Better Mountain Generators That Aren't Perlin Noise or Erosion](https://youtu.be/gsJHzBTPG0Y). (09:27)

Diffusion-limited aggregation (DLA) is a natural phenomenon that can be replicated algorithmically, which looks like this:

<img src="https://github.com/user-attachments/assets/a06d2dc5-ef2f-46d4-b766-078c314a2299" alt="drawing" width="512">

*Source: ![Wikipedia](https://en.wikipedia.org/wiki/Diffusion-limited_aggregation)*

The goal is to create a heightmap using this algorithm, which representents the terrain we want to render. This approach removes
the need to use noise for procedural generation, setting it apart from the traditional method.

The aforementioned video presents a nice visualization of how the algorithm works (at 12:04), and is highly recommended to fully
understand the proposed implementation.

The code is available in the files with the prefix ``dla_`` in the ``src/`` directory and they are commented! Feel free to take a look
and check for yourself, i am open to all kinds of feedback (you can go on my profile to get my email address).

Here are some heightmaps generated with the DLA algorithm:

![DLA_heightmap_1](https://github.com/user-attachments/assets/d2662206-c5c2-476e-b00f-15316c336196)

*512x512, ~5h runtime, (density: 80%, center: middle)​*

![DLA_heightmap_2](https://github.com/user-attachments/assets/fa21d2c2-dd7c-4e0e-8677-7ad046380c62)

*512x512, ~18h runtime, (density: 35%, center: down-right)​*

As you can tell, the main pain point of this algorithm is its runtime. It increases exponentially and stops being viable around
1024x1024 on my machine. The algorithm is bounded to CPU by nature, so optimizations are possible there, but it is not portable
to the GPU. 

These are still some amazing results in my opinion, and i am glad to have experimented with other ways to make procedural terrain
generation!
