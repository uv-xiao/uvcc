#ifndef _REGS_H_
#define _REGS_H_

#include "types.h"
#include "utils.h"
#include "vars.h"
#include "stack.h"

#include <memory>
#include <vector>
#include <map>

namespace obj {


extern std::map<RegID_t, Reg> globalRegs;
extern std::map<Reg, RegID_t> globalRegs_r;

class RegPool;
class RegAllocator;

struct VarInfo {
  std::shared_ptr<EVar> var;
  RegID_t regID = -1; 
  VarInfo() : var(nullptr){}
  VarInfo(evar_sptr var, RegID_t regID) : var(var), regID(regID) {}
  bool operator < (const VarInfo &b) const {
    return *var < *(b.var);
  }
};

class RegPool { 
private:
  static const int REGNUM = 27;
  std::vector<int> status;
  std::map<std::string, int> reg2ID;
  int used;

  void _initialize(int initial = 0) {
    status = std::vector<int>(REGNUM, initial);
    for (auto reg : globalRegs) {
      reg2ID[reg.second] = reg.first;
    }
  }

public:
  RegPool() {
    _initialize();
    status[0] = 1;
    for (int i = 12; i <= 18; i ++) {
      status[i] = 1;
    }
    used = 0;
    for (int i = 0; i < REGNUM; i ++) {
      used += status[i];
    }
  } 

  RegPool(const std::vector<int> freeRegs) {
    _initialize(1);
    used = REGNUM;
    for (auto reg : freeRegs) {
      status[reg] = 0;
      used -= 1;
    }
  }

  int getFreeNum() { return REGNUM - used; }
  int getRegID(const std::string &name) {
    return reg2ID[name];
  }

  RegID_t allocateReg(int preference=-1) {
    if (preference != -1) {
      if (status[preference]) {
        throw std::runtime_error("Preference " + globalRegs[preference] +
                                  "can't be satisfied");
        return -1;
      }
      else {
        status[preference] = 1;
        used ++ ;
        return preference;
      }
    } else {
      if (used == REGNUM) {
        throw std::runtime_error("Not enough free regs");
        return -1;
      }
      for (int i = 0; i < REGNUM; i ++)
        if (!status[i]) {
          status[i] = 1;
          used ++;
          return i;
        }
    }
    return -1;
  }

  void freeReg(RegID_t rid) {
    if (status[rid] == 0) {
      fprintf(stderr, "[WARNING] double free REG[%d]: %s\n",
                      rid, globalRegs[rid].c_str());
    }
    status[rid] = 0;
    used -= 1;
  }

  void dbg_print() {};
};

class RegAllocator {
public:
  struct LiveInfo {
    int start, end;
    evar_sptr  var;
    LiveInfo() : start(0), end(0), var(nullptr) {}
    LiveInfo(int start, int end, evar_sptr var)
      : start(start), end(end), var(var) {}
    bool operator < (const LiveInfo &b) const {
      return end < b.end;
    }
  };
  struct SpillInfo {
    int line;
    evar_sptr var;
    SpillInfo(int line, evar_sptr var)
      : line(line), var(var) {}
    bool operator < (const SpillInfo &b) const {
      return line < b.line; 
    }
  };
  using Active_t = std::set<LiveInfo>;
private:
  RegPool pool;
  std::set<RegID_t> usedRegs;
  std::multiset<SpillInfo> spillSet;
  LiveTable &liveTable;
  StackFrame &stackFrame;
  
  void _insertSpill(int line, evar_sptr var) {
    SpillInfo obj(line, var);
    spillSet.insert(obj);
  }

public: 
  using VarInfoSet = std::set<VarInfo>;
  using Alloc_t = std::map<int, VarInfoSet>;

  RegAllocator(LiveTable &liveTable, StackFrame &stackFrame) 
    : pool(), liveTable(liveTable), stackFrame(stackFrame) {}
  Alloc_t allocate();
  decltype(spillSet.equal_range({1, NULL})) 
      getSpillInfo(int line) {
        return spillSet.equal_range({line, NULL});
      }
  const std::set<RegID_t> &getUsedRegs() const {
    return usedRegs;
  }
};

} // namespace obj
#endif // _REGS_H_ 