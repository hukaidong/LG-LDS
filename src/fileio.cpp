#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#include "fileio.hpp"

std::unique_ptr<char[]> fromfile(const char* D)
{
  FILE *fptr = fopen(D, "rb"); 
  if (!fptr) { std::cerr << D << " not found" << std::endl; exit(3); }

  fseek(fptr, 0, SEEK_END); 
  int length = ftell(fptr); 
  auto buf = std::unique_ptr<char[]>{new char[length+1]};
  fseek(fptr, 0, SEEK_SET); 
  fread(buf.get(), length, 1, fptr); 
  buf[length] = 0; 

  fclose(fptr); 
  return buf; 
}

