#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "public_camera.h"
#include "public_scene.h"
#include "public_image.h"
#include "sphere_tracing.h"
#include <iostream>
#include <cstdlib>

const uint WIDTH = 1000;
const uint HEIGHT = 1000;
const uint MAX_STEPS = 15;
const uint SAMPLES = 10;
const float MIN_THRESHOLD = 0.001f;
const float MAX_THRESHOLD = 1000.f;

int main()
{
  //initializing

  std::vector<float> image;
  image.resize(WIDTH * HEIGHT * 3);

  nsdf::Camera camera;
  camera.from_file("task1_references/cam1.txt");

  nsdf::DirectedLight dir_light;
  dir_light.from_file("task1_references/light.txt");

  nsdf::PrimitiveSDFScene scene;
  scene.from_file("task1_references/scene1.txt");

  for (uint i = 0; i < HEIGHT; ++i)
  {
    for (uint j = 0; j < WIDTH; ++j)
    {
      float3 pixel_color = {0.f, 0.f, 0.f};
      for (uint sample = 0; sample < SAMPLES; ++sample)
      {
        //tracing

        float u_shift = (float) (std::rand() % 100) / 100.f;
        float v_shift = (float) (std::rand() % 100) / 100.f;

        Ray ray;
        ray.dir = camera.generate_ray((float) (j + u_shift) / WIDTH, (float) (i + v_shift) / HEIGHT);
        ray.pos = float3(camera.pos_x, camera.pos_y, camera.pos_z);

        HitRecord hit_record;
        trace(ray, scene.scene_data.data(), scene.scene_data.size() / 4, MAX_STEPS, MIN_THRESHOLD, MAX_THRESHOLD, &hit_record);

        //shading

        float3 color;
        if (hit_record.hit)
        {
          color = {1.f, 1.f, 1.f};
          float light = dot(hit_record.normal, float3(dir_light.dir_x, dir_light.dir_y, dir_light.dir_z)) * dir_light.intensity;
          color = color * clamp(light, 0.f, 1.f);
        }
        else
        {
          color = {0.1f, 0.1f, 0.1f};
        }
        pixel_color += color;
      }

      pixel_color /= SAMPLES;

      image[(i * WIDTH + j) * 3] = pixel_color.x;
      image[(i * WIDTH + j) * 3 + 1] = pixel_color.y;
      image[(i * WIDTH + j) * 3 + 2] = pixel_color.z;
    }
  }

  write_image_rgb("image.png", image, WIDTH, HEIGHT);

  return 0;
}