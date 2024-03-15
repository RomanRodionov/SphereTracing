#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "public_image.h"
#include "sphere_tracing.h"
#include <iostream>
#include <cstdlib>

const uint WIDTH = 1000;
const uint HEIGHT = 1000;

int main()
{
  //initializing

  std::vector<float> image;
  image.resize(WIDTH * HEIGHT * 3);

  nsdf::Camera camera;
  from_file(&camera, "task1_references/cam1.txt");

  nsdf::DirectedLight dir_light;
  from_file(&dir_light, "task1_references/light.txt");

  nsdf::PrimitiveSDFScene scene;
  scene.from_file("task1_references/scene1.txt");

  SphereTracer renderer;
  renderer.draw(scene.scene_data.data(), scene.scene_data.size(), image.data(), camera, dir_light, WIDTH, HEIGHT);

  write_image_rgb("image.png", image, WIDTH, HEIGHT);

  return 0;
}