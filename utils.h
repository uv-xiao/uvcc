#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <cstring>
#include <string>

#define ISA(value, type) type *__val = dynamic_cast<type*>(value)


namespace utils {
  std::vector<std::string> split(const std::string &line, char sep = ' ');
} // namespace utils
#endif // _UTILS_H_
