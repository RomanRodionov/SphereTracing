#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "public_image.h"
#include "public_camera.h"
#include "public_scene.h"
#include "sphere_tracing.h"
#include <iostream>
#include <cstdlib>

const uint WIDTH = 1000;
const uint HEIGHT = 1000;

Camera cast_camera(nsdf::Camera camera)
{
  Camera res;
  res.fov_rad = camera.fov_rad;
  res.pos_x = camera.pos_x;
  res.pos_y = camera.pos_y;
  res.pos_z = camera.pos_z;
  res.target_x = camera.target_x;
  res.target_y = camera.target_y;
  res.target_z = camera.target_z;
  res.up_x = camera.up_x;
  res.up_y = camera.up_y;
  res.up_z = camera.up_z;
  return res;
}

DirectedLight cast_light(nsdf::DirectedLight light)
{
  DirectedLight res;
  res.dir_x = light.dir_x;
  res.dir_y = light.dir_y;
  res.dir_z = light.dir_z;
  res.intensity = light.intensity;
  return res;
}

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
  renderer.draw(scene.scene_data.data(), scene.scene_data.size(), image.data(), cast_camera(camera), cast_light(dir_light), WIDTH, HEIGHT);

  write_image_rgb("image.png", image, WIDTH, HEIGHT);

  return 0;
}