#pragma once
#include <vector>

namespace nsdf
{
  struct PrimitiveSDFScene
  {
    std::vector<float> scene_data; //pos_0.x, pos_0.y, pos_0.z, radius_0, pos_1.x, ...

    bool to_file(const char *filename);
    bool from_file(const char *filename);
  };
}