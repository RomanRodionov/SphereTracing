#pragma once
#include <vector>
#include "public_camera.h"
#include "public_scene.h"
#include "LiteMath.h"
using namespace LiteMath;

class SphereTracer
{
public:
  struct Ray
  {
    float pos_x, pos_y, pos_z;
    float dir_x, dir_y, dir_z;
  };
  struct HitRecord
  {
    float pos_x, pos_y, pos_z;
    float normal_x, normal_y, normal_z;
    uint hit;
  };
  struct Sphere
  {
    float pos_x, pos_y, pos_z, radius; 
  };

  SphereTracer(){}
  virtual void CommitDeviceData(){}
  virtual void GetExecutionTime(const char* a_funcName, float a_out[4]){}

  float3 generate_ray(const nsdf::Camera camera, const float u, const float v, const float aspect_ratio);
  float sphere_distance(Sphere s, Ray r);
  HitRecord trace(Ray ray, const float* scene, uint count, uint steps, float min_threshold, float max_threshold);
  virtual void draw(const float* scene[[size("count")]], uint count, float* image[[size("width*height*3")]],
          nsdf::Camera camera, nsdf::DirectedLight light, uint width, uint height);
  virtual void kernel2D_draw(const float* scene, uint count, float* image, nsdf::Camera camera, nsdf::DirectedLight light, uint width, uint height);
};