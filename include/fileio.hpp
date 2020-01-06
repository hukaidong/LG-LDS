#ifndef FILEIO_H
#define FILEIO_H

#include <memory>

std::unique_ptr<char[]> fromfile(const char* D);

#endif
