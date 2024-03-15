#include "sphere_tracing.h"

float sphere_distance(Sphere s, Ray r)
{
    return length(float3(r.pos_x - s.pos_x, r.pos_y - s.pos_y, r.pos_z - s.pos_z)) - s.radius;
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
            sphere.pos_x = scene[i * 4];
            sphere.pos_y = scene[i * 4 + 1];
            sphere.pos_z = scene[i * 4 + 2];
            sphere.radius = scene[i * 4 + 3];
            float cur_dist = sphere_distance(sphere, ray);
            if (fabs(dist) - fabs(cur_dist) > 0)
            {
                dist = cur_dist;
                closest_sphere = sphere;
            }
        }
        ray.pos_x += ray.dir_x * dist;
        ray.pos_y += ray.dir_y * dist;
        ray.pos_z += ray.dir_z * dist;
        if (fabs(dist) < min_threshold || fabs(dist) > max_threshold)
        {
            step = steps;
        }
    }

    hit_record->hit = fabs(dist) < min_threshold;
    if (hit_record->hit)
    {
        hit_record->pos_x = ray.pos_x;
        hit_record->pos_y = ray.pos_y;
        hit_record->pos_z = ray.pos_z;
        float3 normal = normalize(float3(ray.pos_x - closest_sphere.pos_x, ray.pos_y - closest_sphere.pos_y, ray.pos_z - closest_sphere.pos_z));
        hit_record->normal_x = normal.x;
        hit_record->normal_y = normal.y;
        hit_record->normal_z = normal.z;
    }
    hit_record->pos_x = ray.pos_x;
    hit_record->pos_y = ray.pos_y;
    hit_record->pos_z = ray.pos_z;
}

void SphereTracer::draw(std::vector<float>& image, nsdf::PrimitiveSDFScene scene, nsdf::Camera camera, nsdf::DirectedLight dir_light, uint width, uint height)
{
  kernel2D_draw(image.data(), scene.scene_data.data(), scene.scene_data.size(), camera, dir_light, width, height);
}

void SphereTracer::kernel2D_draw(float* image, const float* scene, uint count, nsdf::Camera camera, nsdf::DirectedLight dir_light, uint width, uint height)
{
  const uint MAX_STEPS = 15;
  const uint SAMPLES = 10;
  const float MIN_THRESHOLD = 0.001f;
  const float MAX_THRESHOLD = 1000.f;
  
  for (uint i = 0; i < height; ++i)
  {
    for (uint j = 0; j < width; ++j)
    {
      float3 pixel_color = {0.f, 0.f, 0.f};
      for (uint sample = 0; sample < SAMPLES; ++sample)
      {
        //tracing

        float u_shift = (float) (std::rand() % 100) / 100.f;
        float v_shift = (float) (std::rand() % 100) / 100.f;

        Ray ray;
        float3 dir = camera.generate_ray((float) (j + u_shift) / width, (float) (i + v_shift) / height);
        ray.dir_x = dir.x;
        ray.dir_y = dir.y;
        ray.dir_z = dir.z;
        ray.pos_x = camera.pos_x;
        ray.pos_y = camera.pos_y;
        ray.pos_z = camera.pos_z;

        HitRecord hit_record;
        trace(ray, scene, count, MAX_STEPS, MIN_THRESHOLD, MAX_THRESHOLD, &hit_record);

        //shading

        float3 color;
        if (hit_record.hit)
        {
          color = {1.f, 1.f, 1.f};
          float light = dot(float3(hit_record.normal_x, hit_record.normal_y, hit_record.normal_z),
                float3(dir_light.dir_x, dir_light.dir_y, dir_light.dir_z)) * dir_light.intensity;
          color = color * clamp(light, 0.f, 1.f);
        }
        else
        {
          color = {0.1f, 0.1f, 0.1f};
        }
        pixel_color += color;
      }

      pixel_color /= SAMPLES;

      image[(i * width + j) * 3] = pixel_color.x;
      image[(i * width + j) * 3 + 1] = pixel_color.y;
      image[(i * width + j) * 3 + 2] = pixel_color.z;
    }
  }
}