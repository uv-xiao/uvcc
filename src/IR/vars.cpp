#include "vars.h"
#include "regs.h"

namespace obj {

GlobalVarManager globalVars;
VarPool globalPool;

template<typename T>
T buildVar(const std::string &str) {
  int value;
  try {
    value = std::stoi(str);
    return std::make_shared<Imm>(value);
  }
  catch (...) {
    return globalPool.getVar(str);
  }
}

// RVar
rvar_sptr RVar::buildRVar(const std::string &str) {
  return buildVar<rvar_sptr>(str);
}

// EVar
evar_sptr EVar::buildEVar(const std::string &str) {
  return globalPool.getVar(str);
}

// GlobalVarManager
void GlobalVarManager::allocateGlobalVar(FILE *f, evar_sptr var, int initval) {
  if (isGlobalVar(var)) 
    throw std::runtime_error("Cannot allocate twice for var " + var->getName());
  tvar_sptr tvar = std::make_shared<TVar>(_name(), 4, false);
  pool[var->getName()] = tvar;
  fprintf(f, "%s = %s\n", tvar->getName().c_str(), 
                          std::to_string(initval).c_str());
  varCount += 1;
}
void GlobalVarManager::allocateGlobalArr(FILE *f, evar_sptr var, int size) {
  if (isGlobalVar(var)) 
    throw std::runtime_error("Cannot allocate twice for var " + var->getName());
  tvar_sptr tvar = std::make_shared<TVar>(_name(), size, true);
  pool[var->getName()] = tvar;
  fprintf(f, "%s = malloc %s\n", tvar->getName().c_str(), 
                          std::to_string(size).c_str());
  varCount += 1;
}

void GlobalVarManager::codegen_loadVar(FILE *f, evar_sptr var, 
                                       const std::string &reg) {
  if (!isGlobalVar(var))
    throw std::runtime_error("Not a global var " + var->getName());
  tvar_sptr tvar = pool[var->getName()];
  if (tvar->isArray()) 
    fprintf(f, "loadaddr ");
  else
    fprintf(f, "load ");
  fprintf(f, "%s %s\n", tvar->getName().c_str(), reg.c_str());
}

void GlobalVarManager::codegen_storeVar(FILE *f, evar_sptr var, 
                                        int offset, int val) {
  if (!isGlobalVar(var))
    throw std::runtime_error("Not a global var " + var->getName());
  tvar_sptr tvar = pool[var->getName()];
  fprintf(f, "    // store global \n");
  fprintf(f, "loadaddr %s t6\n", tvar->getName().c_str());
  fprintf(f, "t6 = t6 + %s\n", globalRegs[offset].c_str());
  fprintf(f, "t6 [0] = %s\n", globalRegs[val].c_str());
}

} // namespace obj