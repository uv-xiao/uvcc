#include "ir.h"
#include "utils.h"
#include "entry.h"

#include <iostream>

int DEBUG_LEVEL = 1;
void testSplit() {
  std::string str("if T0 <= 10 goto l0");
  std::cerr << str << std::endl;
  for (auto sub : utils::split(str, ' ')) {
    std::cerr << sub << std::endl;
  }
}
void testIR(char *inputf = nullptr, char * outputf = nullptr) {

  if (inputf != nullptr)
    freopen(inputf, "r", stdin);
  if (outputf != nullptr)
    freopen(outputf, "w", stdout);
  ir::EInstManager mgr;
  std::string line;
  while (std::getline(std::cin, line)) {
    mgr.addInst(line);
  }
  mgr.codegen(stdout);
}

void runIR() {

}
int main(int argc, char **argv) {
  if (DEBUG_LEVEL == 1) {
    if (argc == 1)
      testIR();
    if (argc == 2)
      testIR(argv[1]);
    if (argc == 3)
      testIR(argv[1], argv[2]);
  } 
  else if (DEBUG_LEVEL == 2) {
    testSplit();
  }
  else {
    runIR();
  }
}