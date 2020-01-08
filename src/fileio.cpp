#include <string>
#include <fstream>
#include <iostream>

#include <cstdio>
#include <cstring>

#include "fileio.hpp"

std::unique_ptr<char[]> fromfile(const char* D)
{
  std::fstream file;
  file.open(D, std::fstream::in);
  std::string s(
      (std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>()
      );


  auto copy = std::make_unique<char[]>(s.size()+1);
  std::strcpy(copy.get(), s.c_str());
  return copy;
}

std::pair<std::vector<GLfloat>, std::vector<GLuint>>
loadObj(const char* fname)  
{
  char sbuf[256];
  float f[3];
  int i[3];
  std::fstream file;
  std::vector<GLfloat> pbuf;
  std::vector<GLuint> vbuf;

  file.open(fname, std::fstream::in);
  while(!file.eof()) {
    if (!file.rdstate() == std::ifstream::goodbit) {
      std::cout << 
        "eof " << file.eof() <<
        ", fail " << file.fail() <<
        ", bad " << file.bad() <<
        std::endl;
      exit(-1);
    }
    file.getline(sbuf, 256);
    if (sbuf[0] == 'v') {
      std::sscanf(sbuf, "%*c %f %f %f", f, f+1, f+2);
      pbuf.insert(pbuf.end(), f, f+3);
      std::cout << 
        "Vertex " << 
        pbuf.size()/3 << ": " <<
        f[0] << ", " <<
        f[1] << ", " <<
        f[2] << 
        std::endl;
    } else if (sbuf[0] == 'f') {
      std::sscanf(sbuf, "%*c %d %d %d", i, i+1, i+2);
      vbuf.insert(vbuf.end(), i, i+3);
      std::cout << 
        "face " << 
        vbuf.size()/3 << ": " <<
        i[0] << ", " <<
        i[1] << ", " <<
        i[2] << 
        std::endl;
    }
  }

  for (auto &v: vbuf) {
    v -= 1;
  }

  return std::make_pair(pbuf, vbuf);
}

