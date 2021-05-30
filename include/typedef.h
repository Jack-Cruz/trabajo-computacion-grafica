#pragma once
#ifndef __TYPEDEF__
#define __TYPEDEF__

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stb_image.h>
#include <path.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace glm;
using namespace std;

typedef float  f32;
typedef double f64;

typedef char      i8;
typedef short int i16;
typedef int       i32;
typedef long long i64;

typedef unsigned char      uchar;
typedef unsigned short int ui16;
typedef unsigned int       ui32;
typedef unsigned long long ui64;

const i32 FSIZE = sizeof(f32);
const i32 ISIZE = sizeof(i32);

#endif 