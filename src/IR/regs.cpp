#include "regs.h"
#include <algorithm>

namespace obj {
std::map<RegID_t, Reg> globalRegs {
  {-1, "ONSTACK!"},
  {0, "x0"},
      /* s reg: callee saved */
  {1, "s0"}, {2, "s1"}, {3, "s2"},
  {4, "s3"}, {5, "s4"}, {6, "s5"},
  {7, "s6"}, {8, "s7"}, {9, "s8"},
  {10, "s9"}, {11, "s10"}, {12, "s11"},
      /* t reg: caller saved */
  {13, "t0"}, {14, "t1"}, {15, "t2"},
  {16, "t3"}, {17, "t4"}, {18, "t5"}, 
      /* a reg: for parameters */
  {19, "a0"}, {20, "a1"}, {21, "a2"},
  {22, "a3"}, {23, "a4"}, {24, "a5"},
  {25, "a6"}, {26, "a7"}
};

std::map<Reg, RegID_t> globalRegs_r {
  {"ONSTACK!", -1},
  {"x0", 0},
      /* s reg: callee saved */
  {"s0", 1}, {"s1", 2}, {"s2", 3},
  {"s3", 4}, {"s4", 5}, {"s5", 6},
  {"s6", 7}, {"s7", 8}, {"s8", 9},
  {"s9", 10}, {"s10", 11}, {"s11", 12},
      /* t reg: caller saved */
  {"t0", 13}, {"t1", 14}, {"t2", 15},
  {"t3", 16}, {"t4", 17}, {"t5", 18}, 
      /* a reg: for parameters */
  {"a0", 19}, {"a1", 20}, {"a2", 21},
  {"a3", 22}, {"a4", 23}, {"a5", 24},
  {"a6", 25}, {"a7", 26}
};

}