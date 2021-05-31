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
RegAllocator::Alloc_t RegAllocator::allocate() {

  if (obj::DEBUG_FLAG) {
    std::cerr << "Register Allocation" << std::endl;
    std::cerr << "  liveTable:" << std::endl;
    for (auto entry : liveTable) {
      std::cerr << "    " << entry.first << ": ";
      for (auto entry1 : entry.second)
        std::cerr << entry1.getName() << " ";
      std::cerr << std::endl;
    }
  }
  Alloc_t ret;
  std::map<EVar, RegID_t> regs;
  
  std::vector<LiveInfo> intervals;

  {
    std::map<EVar, LiveInfo> tmp;
    for (auto entry : liveTable) {
      for (auto var : entry.second) {
        if (!tmp.count(var)) {
          auto pvar = globalPool.getVar(var.getName());
          tmp[var] = LiveInfo(entry.first, entry.first, pvar);
        }
        tmp[var].end = entry.first;
      }
    }
    for (auto entry : tmp) {
      intervals.push_back(entry.second);
    }
  }

  std::sort(intervals.begin(), intervals.end(), 
            [](auto l, auto r) { return l.start < r.start; });
  
  Active_t actives;

  auto ExpireOldInterval = [&](LiveInfo &interval) {

    std::vector<LiveInfo> toErase;
    for (auto active : actives) {
      if (active.end >= interval.start) break;
      else {
        toErase.push_back(active);
        // auto var = active.var;
        // auto rid = regs[*var];
        // pool.freeReg(rid);
        // std::cerr << "Erase from actives: " << active.start << " " 
        //           << active.end << " " << active.var->getName() << std::endl;
        // actives.erase(active);
        // regs.erase(*var);
      }
    }
    for (auto active : toErase) {
        auto var = active.var;
        auto rid = regs[*var];
        pool.freeReg(rid);
        // std::cerr << "Erase from actives: " << active.start << " " 
        //           << active.end << " " << active.var->getName() << std::endl;
        actives.erase(active);
        regs.erase(*var);
    }
  };

  auto SpillAtInterval = [&](LiveInfo &interval) {
    auto spill_ptr = actives.begin();
    auto spill = *spill_ptr;
    if (spill.end > interval.end) {
      regs[*interval.var] = regs[*spill.var];
      _insertSpill(interval.start, spill.var);
      // std::cerr << "Erase from actives: " << spill.start << " " 
      //           << spill.end << " " << spill.var->getName() << std::endl;
      actives.erase(spill_ptr);
      regs[*spill.var] = -1;
      // std::cerr << "Insert to actives : " << interval.start << " "
      //           << interval.end << " " << interval.var->getName() << std::endl;
      actives.insert(interval);
    } else {
      _insertSpill(interval.start, interval.var);
    }
  };

  std::set<int> begins;
  for (auto i : intervals)
    begins.insert(i.start); 
  
  auto UpdateRet = [&](int time) {
    auto &target = ret[time];
    for (auto entry : regs) {
      target.insert({globalPool.getVar(entry.first.getName()), 
                     entry.second});
    }
  };

  for (auto entry : liveTable) {
    int time = entry.first;
    if (begins.count(time) == 0)
      UpdateRet(time);
    else {
      for (auto interval : intervals) {
        if (interval.start == time) {
          pool.dbg_print();
          ExpireOldInterval(interval);
          if (pool.getFreeNum() == 0)
            SpillAtInterval(interval);
          else {
            int rid = -2;
            if (interval.var->getName()[0] == 'p')
              rid = pool.allocateReg(interval.var->getName()[1] - '0' +
                                     pool.getRegID("a0"));
            else
              rid = pool.allocateReg();
            usedRegs.insert(rid);
            // std::cerr << "Insert to actives : " << interval.start << " "
            //           << interval.end << " " << interval.var->getName() << std::endl;
            actives.insert(interval);
            regs[*interval.var] = rid;
          }
        }
      }
      UpdateRet(time);
    }
  }

  return ret;
}

}