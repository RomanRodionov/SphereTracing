#pragma once
#include "LiteMath.h"
using namespace LiteMath;

struct Camera
{
  float pos_x, pos_y, pos_z;
  float target_x, target_y, target_z;
  float up_x, up_y, up_z;
  float fov_rad;
  float z_near;
  float z_far;
};

struct DirectedLight
{
  float dir_x, dir_y, dir_z;
  float intensity;
};

struct Ray
{
  float pos_x, pos_y, pos_z;
  float dir_x, dir_y, dir_z;
};

struct HitRecord
{
  float pos_x, pos_y, pos_z;
  float normal_x, normal_y, normal_z;
  uint hit, a;
};

struct Sphere
{
  float pos_x, pos_y, pos_z, radius; 
};

float3 get_ray(Camera camera, const float u, const float v, const float aspect_ratio);
float sphere_distance(Sphere s, Ray r);
HitRecord trace(Ray ray, const float* scene, uint count, uint steps, float min_threshold, float max_threshold);

class SphereTracer
{
public:
  SphereTracer(){}
  virtual void CommitDeviceData(){}
  virtual void GetExecutionTime(const char* a_funcName, float a_out[4]){}

  virtual void draw(const float* scene[[size("count")]], uint count, float* image[[size("width*height*3")]],
          const Camera camera, const DirectedLight light, uint width, uint height);
  virtual void kernel2D_draw(const float* scene, uint count, float* image, const Camera camera, const DirectedLight light, uint width, uint height);
};