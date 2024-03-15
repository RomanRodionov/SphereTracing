#pragma once
#include "LiteMath.h"
using namespace LiteMath;

struct Ray
{
    float3 pos;
    float3 dir;
};

struct HitRecord
{
    float3 pos;
    float3 normal;
    bool hit;
};

struct Sphere
{
    float3 pos;
    float radius;
};

float sphere_distance(Sphere s, Ray r)
{
    return length(r.pos - s.pos) - s.radius;
}

void trace(Ray ray, const float* scene, uint count, uint steps, float min_threshold, float max_threshold, HitRecord* hit_record)
{
    float dist;
    Sphere closest_sphere;
    for (uint step = 0; step < steps; ++step)
    {
        dist = 1e6f;
        for (uint i = 0; i < count; ++i)
        {
            Sphere sphere;
            sphere.pos = {scene[i * 4], scene[i * 4 + 1], scene[i * 4 + 2]};
            sphere.radius = scene[i * 4 + 3];
            float cur_dist = sphere_distance(sphere, ray);
            if (fabs(dist) - fabs(cur_dist) > 0)
            {
                dist = cur_dist;
                closest_sphere = sphere;
            }
        }
        ray.pos += ray.dir * dist;
        if (fabs(dist) < min_threshold || fabs(dist) > max_threshold)
        {
            step = steps;
        }
    }

    hit_record->hit = fabs(dist) < min_threshold;
    if (hit_record->hit)
    {
        hit_record->pos = ray.pos;
        hit_record->normal = normalize(ray.pos - closest_sphere.pos);
    }
    hit_record->pos = ray.pos;
}