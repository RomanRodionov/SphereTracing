#pragma once
#include "LiteMath.h"
using namespace LiteMath;

namespace nsdf
{
  struct Camera
  {
    float pos_x, pos_y, pos_z; //camera position
    float target_x, target_y, target_z; //point, at which camera is looking
    float up_x, up_y, up_z; //up vector for camera
    float fov_rad; //field of view in radians
    float z_near; //distance to near plane
    float z_far; //distance to far plane
  };

  bool to_file(Camera camera, const char *filename);
  bool from_file(Camera* camera, const char *filename);

  struct DirectedLight
  {
    float dir_x, dir_y, dir_z; //direction TO light, i.e 0,1,0 if light is above
    float intensity;
  };

  bool to_file(DirectedLight light, const char *filename);
  bool from_file(DirectedLight* light, const char *filename);
}