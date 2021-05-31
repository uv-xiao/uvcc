#ifndef _VARS_H_
#define _VARS_H_

#include "types.h"
#include <iostream> 
#include <memory>
#include <cstring>
#include <vector>
#include <set>
#include <map>

namespace obj {

extern VarPool globalPool;
extern GlobalVarManager globalVars;

class RVar {
public:
  virtual std::string getName() const = 0;
  virtual bool operator < (const RVar &b) const {
    return getName() < b.getName();
  }
  virtual bool operator == (const RVar &b) const {
    return getName() == b.getName();
  }
  
  static rvar_sptr buildRVar(const std::string &name);
};

class Imm : public RVar {
private:
  int value;
public:
  Imm(int value) : value(value) {}
  std::string getName() const override { 
    return std::to_string(value);
  }
  int getValue() { return value; }
};

class EVar : public RVar {
private:
  std::string name;
  bool arr = false;
public:
  EVar(const std::string &name) : name(name) {}
  std::string getName() const override {
    return name;
  }
  bool isArray() const { return arr; }
  void setArray() { arr = true; }
  friend class VarPool;
  static evar_sptr buildEVar(const std::string &name);
  bool operator < (const EVar &b) const {
    return name < b.name;
  }
  bool operator == (const EVar &b) const {
    return name == b.name;
  }
};


class TVar {
private:
  std::string name;
  size_t size;
  bool arr;
public:
  TVar(const std::string &name, size_t size, bool arr)
    :name(name), size(size), arr(arr) {}
  std::string getName() const { return name; }
  size_t getSize() const { return size; }
  bool isArray() const { return arr; }
};


class VarPool {
  using evar_sptr = std::shared_ptr<EVar>;
private:
  std::map<std::string, evar_sptr> pool;

public:
  void insertVar(evar_sptr var) {
    pool[var->getName()] = var;
  }
  evar_sptr getVar(const std::string &name) {
    if (pool.find(name) == pool.end()) {
      insertVar(std::make_shared<EVar>(name));
    }
    return pool[name];
  }
};



class GlobalVarManager { 
private:
  std::map<std::string, tvar_sptr> pool;
  int varCount = 0;
  std::string _name() {
    return "v" + std::to_string(varCount);
  }
public:
  bool isGlobalVar(evar_sptr var) {
    if (pool.count(var->getName())) return true;
    else return false;
  }
  std::string getVarName(evar_sptr var) {
    if (!isGlobalVar(var)) {
      std::cerr << "Error: Cannot find global var " + var->getName() 
                << std::endl;
      return "";
    }
    else return pool[var->getName()]->getName();
  }
  size_t getGlobalVarCount() {
    return pool.size();
  }
  std::vector<tvar_sptr> getAllTVars() {
    std::vector<tvar_sptr> ret;
    return ret;
  }
  std::vector<evar_sptr> getAllEVars() {
    std::vector<evar_sptr> ret;
    for (auto &entry: pool) 
      ret.push_back(globalPool.getVar(entry.first));
    return ret;
  }

  void codegen_storeVar(FILE *f, evar_sptr var, int offreg, int vallreg);
  void codegen_loadVar(FILE *f, evar_sptr var, const std::string &reg);

  void allocateGlobalVar(FILE *f, evar_sptr var, int initval = 0);
  void allocateGlobalArr(FILE *f, evar_sptr var, int size);
};

  
} // namespace obj
#endif // _VARS_H_