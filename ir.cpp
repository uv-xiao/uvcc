#include "ir.h"
#include "types.h"
#include "regs.h"
#include "utils.h"
#include <algorithm>

namespace obj {
  
} // namespace obj

namespace ir {

// EParam
EParam::EParam(stringvec &vec, int lineno, f_sptr f) 
  : EInst(lineno, f), var(obj::RVar::buildRVar(vec[1])) {}

// EReturn
EReturn::EReturn(stringvec &vec, int lineno, f_sptr f) 
    : EInst(lineno, f), var(obj::RVar::buildRVar(vec[1])) {}

// EDefArr
EDefArr::EDefArr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), var(obj::EVar::buildEVar(vec[2])) {
  var->setArray();
  len = std::stoi(vec[1]);
}

// EAssign
EAssign::EAssign(const stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0]))
    , rvar(obj::RVar::buildRVar(vec[2])) {
}

std::string EAssign::codegen(FILE *f)  {
  auto lreg = this->f->getRegFromMem(f, lvar);
  std::string rreg;
  if (ISA(rvar.get(), obj::Imm)) {
    rreg = rvar->getName();
  } else {
    rreg = this->f->getRegFromMem(f, rvar);
  }

  fprintf(f, "%s = %s\n", lreg.c_str(), rreg.c_str());
  this->f->backToMemory(f, lvar, obj::globalRegs_r[lreg]);
  return "";
}

// EStoreArr
EStoreArr::EStoreArr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0])),
      rvar(obj::RVar::buildRVar(vec[3])) {
  vec[1].pop_back();
  offset = obj::RVar::buildRVar(vec[1].substr(1));
}

// EUnaryExpr
EUnaryExpr::EUnaryExpr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0])),
      op(vec[2]), rvar(obj::EVar::buildRVar(vec[3])) {}

// ELoadArr
ELoadArr::ELoadArr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0])),
      rvar(obj::EVar::buildEVar(vec[2])) {
    vec[3].pop_back();
    offset = obj::RVar::buildRVar(vec[3].substr(1));
} 

// EBinaryExpr
EBinaryExpr::EBinaryExpr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0])),
      rvar1(obj::RVar::buildRVar(vec[2])),
      rvar2(obj::RVar::buildRVar(vec[4])),
      op(vec[3]) {}
std::string EBinaryExpr::codegen(FILE *f)  {
  auto lreg = this->f->getRegFromMem(f, lvar);
  std::string r1, r2;
  r1 = this->f->getRegFromMem(f, rvar1, 1);
  r2 = this->f->getRegFromMem(f, rvar2, 1);
  
  fprintf(f, "%s = %s %s %s\n", lreg.c_str(), r1.c_str(), 
                                op.c_str(), r2.c_str());

  this->f->backToMemory(f, lvar, obj::globalRegs_r[lreg]);
  return "";
}

// ECJump
ECJump::ECJump(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), rvar1(obj::RVar::buildRVar(vec[1])),
      rvar2(obj::RVar::buildRVar(vec[3])), lop(vec[2]), label(vec[5]) {}


// class Function

Function::Function(mgr_sptr mgr) {
  instMgr = mgr;
  tmpPool = obj::RegPool(std::move(std::vector<int>{13, 14, 15, 16, 17, 18}));
  paramCount = -1;
  tempParamCount = 0;
}

void Function::setupBasic() {
  int size = 0;
  size = paramCount * 4;
  for (auto inst : instTable) {
    if (ISA(inst.second.get(), EDefVar)) {
      size += 4;
    }
    else if (ISA(inst.second.get(), EDefArr)) {
      size += std::dynamic_pointer_cast<EDefArr>(inst.second)->getLength();
    }
  }
  stackFrame.expand(size);
  // stackFrame.expand(obj::globalVars.getGlobalVarCount() * 4);

  for (int i = 0; i < paramCount; i++) {
    stackFrame.allocateVar(std::make_shared<obj::EVar>(
        std::string("p")+std::to_string(i)));
  }
  
  for (auto inst : instTable) {
    if (ISA(inst.second.get(), EDefVar)) {
      std::shared_ptr<EDefVar>
          ptr = std::dynamic_pointer_cast<EDefVar>(inst.second); 
      int tmp = stackFrame.allocateVar(
        std::dynamic_pointer_cast<obj::EVar>(ptr->getVar()));
    }
    else if (ISA(inst.second.get(), EDefArr)) {
      std::shared_ptr<EDefArr>
          ptr = std::dynamic_pointer_cast<EDefArr>(inst.second);
      stackFrame.allocateArr(
        ptr->getVar(), ptr->getLength());
    }
  }
}

void Function::addInst(i_sptr inst) { instTable[inst->getLine()] = inst; }

void Function::codegen(FILE *f) {
  obj::VarList liveVars;

  for (auto inst : instTable) {
    // debug info
    std::string code = inst.second->getCode();
    if (code.back() == '\n') code.pop_back();
    fprintf(f, "  // line: %s -> %s\n", 
        std::to_string(inst.first).c_str(), 
        code.c_str());
    std::cerr << "line: " << inst.first <<  " -> " <<  code << std::endl;
    inst.second->codegen(f);
  }
}

std::string Function::getRegFromMem(FILE *f, rvar_sptr rvar, bool mustReg) {
  static int curReg = 0;
  static std::string candidates[6] = {"t0", "t1", "t2", "t3", "t4", "t5"};

  if (ISA(rvar.get(), obj::Imm)) {
    std::shared_ptr<obj::Imm> imm = std::dynamic_pointer_cast<obj::Imm>(rvar);
    if (mustReg) {
      std::string reg = candidates[curReg ++];
      curReg %= 6;
      fprintf(f, "%s = %d\n", reg.c_str(), imm->getValue());
      return reg;
    }
    else
      return std::to_string(imm->getValue());
  }
  else {
    std::string reg;
    evar_sptr evar = std::dynamic_pointer_cast<obj::EVar>(rvar);
    reg = candidates[curReg ++]; 
    curReg %= 6;
  
    if (evar->isArray()) {
      // Here loading address
      if (obj::globalVars.isGlobalVar(evar)) {
        obj::globalVars.codegen_loadVar(f, evar, reg);
      }
      else {
        auto offset = stackFrame.addressOfVar(evar);
        stackFrame.codegen_lea(f, offset, tmpPool.getRegID(reg));
      }
    }
    else {
      if (obj::globalVars.isGlobalVar(evar))
        obj::globalVars.codegen_loadVar(f, evar, reg);
      else {
        auto offset = stackFrame.addressOfVar(evar);
        stackFrame.codegen_spillout(f, offset, tmpPool.getRegID(reg));
      }
    }

    return reg;
  }
}

std::string Function::prepareParam(FILE *f) {
  auto reg = obj::globalRegs[tempParamCount + 19];
  tempParamCount += 1;
  return reg;
}


void Function::startFunc(FILE *f)  {
  for (int i = 0; i < paramCount; i ++) {
    int offset = 27 + i;
    stackFrame.codegen_spillin(f, offset, 19 + i);
  }
}

void Function::prepareCall(FILE *f) {
  tempParamCount = 0;
}
void Function::endCall(FILE *f) {
  tempParamCount = 0;
}

void Function::backToStack(FILE *f, evar_sptr lvar, int reg, int offreg) {
  auto offset = stackFrame.addressOfVar(lvar);
  stackFrame.codegen_spillin(f, offset, reg, offreg);
}

void Function::backToGlobal(FILE *f, evar_sptr gvar, int reg, int offreg) {
  obj::globalVars.codegen_storeVar(f, gvar, offreg, reg);
}

void Function::backToMemory(FILE *f, evar_sptr var, int reg, int offreg) {
  if (obj::globalVars.isGlobalVar(var)) {
    backToGlobal(f, var, reg, offreg);
  }
  else {
    backToStack(f, var, reg, offreg);
  }
}

void Function::backToAddress(FILE *f, evar_sptr addr, int reg, int offreg) {
  fprintf(f, "load %s t6\nt6 = t6 + %s\nt6 [0] = %s\n",
            std::to_string(stackFrame.addressOfVar(addr)).c_str(),
            obj::globalRegs[offreg].c_str(),
            obj::globalRegs[reg].c_str());
}

// class EInstManager
const std::string EInstManager::GLOBAL_RESMGR_NAME = "global";
const std::string EInstManager::GLOBAL_SCOPE_NAME = "f__init";

void EInstManager::addInst(const std::string &line) {
#define ADDI(type) _addInst(std::make_shared<type>(vec, curLine, getFunc(curFunc)))

  std::cerr << line << " -> ";
  input.push_back(line);
  stringvec vec = utils::split(line, ' ');
  for (auto str : vec)
    std::cerr << str << " ";
  std::cerr << std::endl;

  if (vec.size() == 0) return;
  switch(vec.size()) {
    case 0:
      break;
    case 1:
      if(vec[0] == "return")
        ADDI(ERetVoid);
      else if (vec[0].back() == ':') {
        labelTable[vec[0]] = curLine;
        ADDI(ELable);
      }
      else {
        std::cout << vec[0] << ": " << (vec[0].back() == ':') << std::endl;
        goto ERROR;
      }
      break;

    case 2:
      if (vec[0] == "goto")
        ADDI(EJump);
      else if (vec[0] == "var")
        ADDI(EDefVar);
      else if (vec[0] == "end") {
        ADDI(EEndFunc);
        endScope();
      }
      else if (vec[0] == "param")
        ADDI(EParam);
      else if (vec[0] == "return")
        ADDI(EReturn);
      else if (vec[0].substr(0,2) == "f_") {
        newScope(vec[0]);
        auto in = std::dynamic_pointer_cast<EFunc>(ADDI(EFunc));
        auto param = in->getParam();
        param.pop_back();
        funcTable[curFunc]->paramCount = std::stoi(param.substr(1));
      }
      else if (vec[0] == "call")
        ADDI(ECallVoid);
      else goto ERROR;
      break;

    case 3:
      if (vec[1] == "=")
        ADDI(EAssign);
      else if (vec[0] == "var")
        ADDI(EDefArr);
      else goto ERROR;
      break;

    case 4:
      if (vec[2] == "call")
        ADDI(ECall);
      else if (vec[1] == "=" && vec[3][0] == '[')
        ADDI(ELoadArr);
      else if (vec[1] == "=")
        ADDI(EUnaryExpr);
      else if (vec[2] == "=")
        ADDI(EStoreArr);
      else goto ERROR;
      break;

    case 6:
      if(vec[0] == "if")
        ADDI(ECJump);
      else goto ERROR;
      break;

    case 5:
      if (vec[1] == "=")
        ADDI(EBinaryExpr);
      else goto ERROR;
      break;

    default:
ERROR:
    std::cerr << "syntax error!" << std::endl;
    throw std::runtime_error("Invalid Grammar at line: " + 
                            std::to_string(curLine) + ": " +
                            line + "| Len: "+std::to_string(vec.size()));
  };
#undef ADDI
}

int EInstManager::getLabel(const std::string &label) {
  int val = labelTable[label + ":"];
  if (val == 0) std::runtime_error("Label " + label + " not found");
  return val + 1;
}

void EInstManager::newScope(const std::string &scopeName) {
  curFunc = scopeName;
  funcTable[curFunc] = std::make_unique<Function>(
                          std::make_shared<EInstManager>(*this));
}

void EInstManager::endScope() {
  curFunc = GLOBAL_SCOPE_NAME;
}

void EInstManager::codegen(FILE *f) {
  std::cerr << "start codegen" << std::endl;
  auto &initFunc = *funcTable[GLOBAL_SCOPE_NAME].get();
  for (auto entry : initFunc.getInsts()) {
    auto inst = entry.second;
    if (ISA(inst.get(), EDefVar)) {
      auto defvar = std::dynamic_pointer_cast<EDefVar>(inst);
      // if (defvar->getVar()->getName()[0] == 'T') {
      obj::globalVars.allocateGlobalVar(f, defvar->getVar(), 0);
      // }
    }
    else if (ISA(inst.get(), EDefArr)) {
      auto defarr = std::dynamic_pointer_cast<EDefArr>(inst);
      obj::globalVars.allocateGlobalArr(f, defarr->getVar(), 
                                        defarr->getLength());
    }
    else {
      auto fmain = getFunc("f_main");
      fmain->addInst(inst);
      inst->setFunc(fmain);
    }
  }
  funcTable.erase(GLOBAL_SCOPE_NAME);
  for (auto entry : funcTable) {
    std::cerr << "parsing " +  entry.first << std::endl;

    entry.second->setupBasic();
    entry.second->codegen(f);
  }
}

} // namespace ir