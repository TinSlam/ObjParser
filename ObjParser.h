#pragma once

#include <map>

extern void parseFile(const char* path, float** vbo, int* vboLength, short** ibo, int* iboLength, float* width, float* height, float* depth, std::map<int, std::string>** materialsMap, int** materials, int** materialsIndices, int* materialsNumber);