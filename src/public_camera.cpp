#include "public_camera.h"
#include <cstring>
#include <cstdio>
#include <errno.h>

namespace nsdf
{
    bool to_file(Camera camera, const char *filename)
    {
      FILE *f = fopen(filename, "w");
      if (!f)
      {
        fprintf(stderr, "failed to open/create file %s. Errno %d\n",filename, (int)errno);
        return false;
      }
      fprintf(f, "camera_position = %f, %f, %f\n", camera.pos_x, camera.pos_y, camera.pos_z);
      fprintf(f, "target = %f, %f, %f\n", camera.target_x, camera.target_y, camera.target_z);
      fprintf(f, "up = %f, %f, %f\n", camera.up_x, camera.up_y, camera.up_z);
      fprintf(f, "field_of_view  = %f\n", camera.fov_rad);
      fprintf(f, "z_near  = %f\n", camera.z_near);
      fprintf(f, "z_far  = %f\n", camera.z_far);

      int res = fclose(f);
      if (res != 0)
      {
        fprintf(stderr, "failed to close file %s. fclose returned %d\n",filename, res);
        return false;
      }
      return true;
    }

    bool from_file(Camera* camera, const char *filename)
    {
      FILE *f = fopen(filename, "r");
      if (!f)
      {
        fprintf(stderr, "failed to open file %s. Errno %d\n",filename, (int)errno);
        return false;
      }
      fscanf(f, "camera_position = %f, %f, %f\n", &camera->pos_x, &camera->pos_y, &camera->pos_z);
      fscanf(f, "target = %f, %f, %f\n", &camera->target_x, &camera->target_y, &camera->target_z);
      fscanf(f, "up = %f, %f, %f\n", &camera->up_x, &camera->up_y, &camera->up_z);
      fscanf(f, "field_of_view  = %f\n", &camera->fov_rad);
      fscanf(f, "z_near  = %f\n", &camera->z_near);
      fscanf(f, "z_far  = %f\n", &camera->z_far);

      int res = fclose(f);
      if (res != 0)
      {
        fprintf(stderr, "failed to close file %s. fclose returned %d\n",filename, res);
        return false;
      }
      return true;
    }

    bool to_file(DirectedLight light, const char *filename)
    {
      FILE *f = fopen(filename, "w");
      if (!f)
      {
        fprintf(stderr, "failed to open/create file %s. Errno %d\n",filename, (int)errno);
        return false;
      }
      fprintf(f, "light direction = %f, %f, %f\n", light.dir_x, light.dir_y, light.dir_z);
      fprintf(f, "intensity = %f\n", light.intensity);

      int res = fclose(f);
      if (res != 0)
      {
        fprintf(stderr, "failed to close file %s. fclose returned %d\n",filename, res);
        return false;
      }
      return true;
    }

    bool from_file(DirectedLight* light, const char *filename)
    {
      FILE *f = fopen(filename, "r");
      if (!f)
      {
        fprintf(stderr, "failed to open file %s. Errno %d\n",filename, (int)errno);
        return false;
      }
      fscanf(f, "light direction = %f, %f, %f\n", &light->dir_x, &light->dir_y, &light->dir_z);
      fscanf(f, "intensity = %f\n", &light->intensity);

      int res = fclose(f);
      if (res != 0)
      {
        fprintf(stderr, "failed to close file %s. fclose returned %d\n",filename, res);
        return false;
      }
      return true;
    }
}