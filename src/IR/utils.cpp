#include "utils.h"

#include <iostream>
#include <cstdio>
#include <sstream>


namespace utils{
std::vector<std::string> split(const std::string &s, char sep) {
    std::stringstream sin(s);
    std::vector<std::string> ret;
    // sin.str(s);
    std::string part;
    char ch;
    while (sin.get(ch)) {
      // std::cerr << ch << std::endl;
      if ((ch == '[')) {
        if (part.length() > 0)
          ret.push_back(part);
        part = "[";
      }
      else if (ch == ']') {
        part = part + "]";
        ret.push_back(part);
        part = "";
      }
      else if ((ch == sep || ch == '\n')) {
        if (part.length() > 0)
          ret.push_back(part);
        part = "";
      }
      else if (ch == '\r')
        continue;
      else {
        part = part + ch;
      }      
    }
    if (part.length()) {
      ret.push_back(part);
    }
    // while(std::getline(sin, part, sep))
    // {
    //     while (part.back() == '\n' || part.back() == ' ' || 
    //            part.back() == '\r') {
    //       part.erase(part.end() - 1);
    //     }
    //     if(part.length())
    //         ret.emplace_back(std::move(part));
    // }
    return ret;
}

} // namespace utils
