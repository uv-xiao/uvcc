#include "entry.h"
#include <cstring>
#include <iostream>

int DEBUG_LEVEL = 0;

void testRISCV(char *indir = nullptr, char *outdir = nullptr) {
  if (indir != nullptr)
    freopen(indir, "r", stdin);
  if (outdir != nullptr)
    freopen(outdir, "w", stdout);
  riscv::RISCVGen gen;
  std::string line;
  while (std::getline(std::cin, line))
    gen.addLine(line);
  gen.codegen(stdout);  
}

int main(int argc, char **argv) {
  if (DEBUG_LEVEL) {
    testRISCV(argv[1], argv[2]);
  } else {
    testRISCV(argv[2], argv[4]);
  }

}