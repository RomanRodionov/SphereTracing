#pragma once
#include <cstring>
#include <cstdio>
#include <errno.h>
#include "LiteMath.h"
using namespace LiteMath;

namespace nsdf
{
  struct Camera
  {
    float pos_x, pos_y, pos_z; //camera position
    float target_x, target_y, target_z; //point, at which camera is looking
    float up_x, up_y, up_z; //up vector for camera
    float fov_rad = 3.14159265f / 3; //field of view in radians
    float z_near = 0.1f; //distance to near plane
    float z_far = 100.0f; //distance to far plane

    float3 generate_ray(float u, float v, float aspect_ratio = 1.f)
    {
      double viewport_width = 2.0 * tan(fov_rad / 2.0) * z_near;
      double viewport_height = viewport_width / aspect_ratio;
      
      float3 w_dir = normalize(float3(target_x - pos_x, target_y - pos_y, target_z - pos_z));
      float3 u_dir = normalize(-cross(float3(up_x, up_y, up_z), w_dir));
      float3 v_dir = cross(w_dir, u_dir);

      float3 pos = float3(pos_x, pos_y, pos_z);
      float3 horizontal = u_dir * viewport_width;
      float3 vertical = v_dir * viewport_height;
      float3 ll_corner = pos - horizontal / 2 - vertical / 2 + w_dir * z_near;

      return normalize(ll_corner + horizontal * u + vertical * v - pos);
    }

    bool to_file(const char *filename)
    {
      FILE *f = fopen(filename, "w");
      if (!f)
      {
        fprintf(stderr, "failed to open/create file %s. Errno %d\n",filename, (int)errno);
        return false;
      }
      fprintf(f, "camera_position = %f, %f, %f\n", pos_x, pos_y, pos_z);
      fprintf(f, "target = %f, %f, %f\n", target_x, target_y, target_z);
      fprintf(f, "up = %f, %f, %f\n", up_x, up_y, up_z);
      fprintf(f, "field_of_view  = %f\n", fov_rad);
      fprintf(f, "z_near  = %f\n", z_near);
      fprintf(f, "z_far  = %f\n", z_far);

      int res = fclose(f);
      if (res != 0)
      {
        fprintf(stderr, "failed to close file %s. fclose returned %d\n",filename, res);
        return false;
      }
      return true;
    }

    bool from_file(const char *filename)
    {
      FILE *f = fopen(filename, "r");
      if (!f)
      {
        fprintf(stderr, "failed to open file %s. Errno %d\n",filename, (int)errno);
        return false;
      }
      fscanf(f, "camera_position = %f, %f, %f\n", &pos_x, &pos_y, &pos_z);
      fscanf(f, "target = %f, %f, %f\n", &target_x, &target_y, &target_z);
      fscanf(f, "up = %f, %f, %f\n", &up_x, &up_y, &up_z);
      fscanf(f, "field_of_view  = %f\n", &fov_rad);
      fscanf(f, "z_near  = %f\n", &z_near);
      fscanf(f, "z_far  = %f\n", &z_far);

      int res = fclose(f);
      if (res != 0)
      {
        fprintf(stderr, "failed to close file %s. fclose returned %d\n",filename, res);
        return false;
      }
      return true;
    }
  };

  struct DirectedLight
  {
    float dir_x, dir_y, dir_z; //direction TO light, i.e 0,1,0 if light is above
    float intensity = 1.0f;

    bool to_file(const char *filename)
    {
      FILE *f = fopen(filename, "w");
      if (!f)
      {
        fprintf(stderr, "failed to open/create file %s. Errno %d\n",filename, (int)errno);
        return false;
      }
      fprintf(f, "light direction = %f, %f, %f\n", dir_x, dir_y, dir_z);
      fprintf(f, "intensity = %f\n", intensity);

      int res = fclose(f);
      if (res != 0)
      {
        fprintf(stderr, "failed to close file %s. fclose returned %d\n",filename, res);
        return false;
      }
      return true;
    }

    bool from_file(const char *filename)
    {
      FILE *f = fopen(filename, "r");
      if (!f)
      {
        fprintf(stderr, "failed to open file %s. Errno %d\n",filename, (int)errno);
        return false;
      }
      fscanf(f, "light direction = %f, %f, %f\n", &dir_x, &dir_y, &dir_z);
      fscanf(f, "intensity = %f\n", &intensity);

      int res = fclose(f);
      if (res != 0)
      {
        fprintf(stderr, "failed to close file %s. fclose returned %d\n",filename, res);
        return false;
      }
      return true;
    }
  };
}