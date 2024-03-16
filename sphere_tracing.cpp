#include "sphere_tracing.h"


float3 get_ray(Camera camera, float u, float v, float aspect_ratio)
{
  float viewport_height = 2.0 * std::atan(camera.fov_rad / 2.0) * camera.z_near;
  float viewport_width = viewport_height * aspect_ratio;
  
  float3 w_dir = normalize(float3(camera.target_x - camera.pos_x, camera.target_y - camera.pos_y, camera.target_z - camera.pos_z));
  float3 u_dir = normalize(cross(float3(camera.up_x, camera.up_y, camera.up_z), w_dir) * (-1));
  float3 v_dir = cross(w_dir, u_dir);

  float3 pos = float3(camera.pos_x, camera.pos_y, camera.pos_z);
  float3 horizontal = u_dir * viewport_width;
  float3 vertical = v_dir * viewport_height;
  float3 ll_corner = pos - horizontal / 2 - vertical / 2 + w_dir * camera.z_near;

  return normalize(ll_corner + horizontal * u + vertical * v - pos);
}

float sphere_distance(Sphere s, Ray r)
{
    return length(float3(r.pos_x - s.pos_x, r.pos_y - s.pos_y, r.pos_z - s.pos_z)) - s.radius;
}

HitRecord trace(Ray ray, const float* scene, uint count, uint steps, float min_threshold, float max_threshold)
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

    HitRecord hit_record;
    hit_record.hit = fabs(dist) < min_threshold;
    if (hit_record.hit)
    {
        hit_record.pos_x = ray.pos_x;
        hit_record.pos_y = ray.pos_y;
        hit_record.pos_z = ray.pos_z;
        float3 normal = normalize(float3(ray.pos_x - closest_sphere.pos_x, ray.pos_y - closest_sphere.pos_y, ray.pos_z - closest_sphere.pos_z));
        hit_record.normal_x = normal.x;
        hit_record.normal_y = normal.y;
        hit_record.normal_z = normal.z;
    }
    hit_record.pos_x = ray.pos_x;
    hit_record.pos_y = ray.pos_y;
    hit_record.pos_z = ray.pos_z;
    return hit_record;
}

void SphereTracer::draw(const float* scene, float* image,
  uint count, uint width, uint height, const Camera camera, const DirectedLight light)
{
  kernel2D_draw(scene, image, count, width, height, camera, light);
}

void SphereTracer::kernel2D_draw(const float* scene, float* image, uint count, uint width, uint height, const Camera camera, const DirectedLight dir_light)
{  
  for (uint i = 0; i < height; ++i)
  {
    for (uint j = 0; j < width; ++j)
    {
      const uint max_steps = 15;
      const uint samples = 10;
      const float min_threshold = 0.001f;
      const float max_threshold = 1000.f;
      float aspect_ratio = (float) width / height; 

      float3 pixel_color = {0.f, 0.f, 0.f};
      for (uint s = 0; s < samples; ++s)
      {
        //tracing

        float u_shift = (float) (50 % 100) / 100.f;
        float v_shift = (float) (50 % 100) / 100.f;

        Ray ray;
        float3 dir = get_ray(camera, (float) (j + u_shift) / width, (float) (i + v_shift) / height, aspect_ratio);
        ray.dir_x = dir.x;
        ray.dir_y = dir.y;
        ray.dir_z = dir.z;
        ray.pos_x = camera.pos_x;
        ray.pos_y = camera.pos_y;
        ray.pos_z = camera.pos_z;

        HitRecord hit_record = trace(ray, scene, count, max_steps, min_threshold, max_threshold);

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

      pixel_color /= samples;

      image[(i * width + j) * 3] = pixel_color.x;
      image[(i * width + j) * 3 + 1] = pixel_color.y;
      image[(i * width + j) * 3 + 2] = pixel_color.z;
    }
  }
}