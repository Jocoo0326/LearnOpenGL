// LuaDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <lua.hpp>

#include <iostream>
#include <string>
#include <sstream>

#define _max 256
typedef struct _ph_func
{
  char ph_name[_max];
  int(*ph_p_func)(lua_State*);
} ph_func;

#define custom_lua_func_struct(fname) #fname, &ph_##fname
#define custom_lua_function_dec(fname) int ph_##fname(lua_State* L)
#define arr_len(arr) (sizeof(arr) / sizeof(arr[0]))

custom_lua_function_dec(test_func);
int main(int argc, const char* argv[])
{
  int r;
  const char* err;
  lua_State* ls;

  ls = luaL_newstate();
  luaL_openlibs(ls);

  luaL_Reg p_funcs[] = {
    { custom_lua_func_struct(test_func) },
  };
  for (size_t i = 0; i < arr_len(p_funcs); i++)
  {
    lua_register(ls, p_funcs[i].name, p_funcs[i].func);
  }

  r = luaL_loadfile(ls, argv[1]);
  if (r)
  {
    err = lua_tostring(ls, -1);
    if (err)
    {
      std::cout << err << std::endl;
      return 1;
    }
  }
  r = lua_pcall(ls, 0, 0, 0);
  if (r)
  {
    err = lua_tostring(ls, -1);
    if (err)
    {
      std::cout << err << std::endl;
      return 1;
    }
  }
  lua_close(ls);
  std::getchar();
  return 0;
}

custom_lua_function_dec(test_func)
{
  const char* p1 = lua_tostring(L, 1); // first param
  const char* p2 = lua_tostring(L, 2); // second param

  std::stringstream ss;
  ss << p1 << p2;
  lua_pushstring(L, ss.str().c_str()); // push result
  return 1; // return number of results
}
