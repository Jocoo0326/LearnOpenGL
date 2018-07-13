#ifndef utils_h
#define utils_h

#include "config.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace utils
{
  void print(const char* msg);

  template<typename T>
  T clamp(T curValue, T minValue, T maxValue)
  {
    return std::max(minValue, std::min(maxValue, curValue));
  }
}

#endif // utils_h
