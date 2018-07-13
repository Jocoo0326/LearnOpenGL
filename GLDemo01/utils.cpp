#include "utils.h"

#include <iostream>
#include <Windows.h>

namespace utils
{
  void print(const char* msg)
  {
#ifdef DEBUG
    OutputDebugString("\n");
    OutputDebugString(msg);
    OutputDebugString("\n");
#endif // DEBUG
  }
}