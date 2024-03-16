#pragma once
#include "LiteMath.h"
using namespace LiteMath;

struct Camera
{
  float pos_x, pos_y, pos_z;
  float target_x, target_y, target_z;
  float up_x, up_y, up_z;
  float fov_rad, z_near, z_far;
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
  bool hit;
};

struct Sphere
{
  float pos_x, pos_y, pos_z, radius; 
};

class SphereTracer
{
public:
  SphereTracer(){}
  virtual void CommitDeviceData(){}
  virtual void GetExecutionTime(const char* a_funcName, float a_out[4]){}

  virtual void draw(const float* scene[[size("count")]], float* image[[size("width*height*3")]], uint count, uint width, uint height,
          const Camera camera, const DirectedLight light);
  virtual void kernel2D_draw(const float* scene, float* image, uint count, uint width, uint heightZ, const Camera camera, const DirectedLight light,
          const Ray ray, const HitRecord hit, const Sphere s);
};