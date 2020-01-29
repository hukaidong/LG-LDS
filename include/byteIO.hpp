#ifndef FILEIO_H
#define FILEIO_H

#include <memory>

std::unique_ptr<char[]> GetBytesFromFile(const char* D);

#endif
