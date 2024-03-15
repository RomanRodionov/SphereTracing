#pragma once
#include <vector>
#include "public_camera.h"
#include "public_scene.h"
#include "LiteMath.h"
using namespace LiteMath;

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

class SphereTracer
{
public:
    SphereTracer(){}
    virtual void draw(std::vector<float>& image, nsdf::PrimitiveSDFScene scene, nsdf::Camera camera, nsdf::DirectedLight light, uint width, uint height);
    virtual void kernel2D_draw(float* image, const float* scene, uint count, nsdf::Camera camera, nsdf::DirectedLight light, uint width, uint height);
};