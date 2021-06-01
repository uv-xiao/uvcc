#include "ir.h"
#include "types.h"
#include "regs.h"
#include "utils.h"
#include <algorithm>

namespace obj {
  
} // namespace obj

namespace ir {


// class EIsnt
// int EInst::getPrevLineno() {
//   i_sptr current = makeThis();
//   i_sptr prev = f->getPrevInst(current);
//   if (prev) {
//     return prev->getLine();
//   } else
//     return 0;
// }

// EParam
EParam::EParam(stringvec &vec, int lineno, f_sptr f) 
  : EInst(lineno, f), var(obj::RVar::buildRVar(vec[1])) {
  // if (ISA(var.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(var).get());
  // }
}

// EReturn
EReturn::EReturn(stringvec &vec, int lineno, f_sptr f) 
    : EInst(lineno, f), var(obj::RVar::buildRVar(vec[1])) {
  // if (ISA(var.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(var).get());
  // }
}

// EDefArr
EDefArr::EDefArr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), var(obj::EVar::buildEVar(vec[2])) {
  var->setArray();
  len = std::stoi(vec[1]);
  // def.insert(*var.get());
}

// EAssign
EAssign::EAssign(const stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0]))
    , rvar(obj::RVar::buildRVar(vec[2])) {
  // def.insert(*lvar.get());
  // if (ISA(rvar.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(rvar).get());
  // }
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
  // if (lreg[0] == 't' || lreg[0] == 'T' || obj::globalVars.isGlobalVar(lvar)) {
  //   this->f->backToMemory(f, lvar, obj::globalRegs_r[lreg]);
  // }
  return "";
}

// EStoreArr
EStoreArr::EStoreArr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0])),
      rvar(obj::RVar::buildRVar(vec[3])) {
  vec[1].pop_back();
  offset = obj::RVar::buildRVar(vec[1].substr(1));
  // use.insert(*lvar.get());
  // if (ISA(rvar.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(rvar).get());
  // }
  // if (ISA(offset.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(offset).get());
  // }
}

// EUnaryExpr
EUnaryExpr::EUnaryExpr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0])),
      op(vec[2]), rvar(obj::EVar::buildRVar(vec[3])) {
  // def.insert(*lvar.get());
  // if (ISA(rvar.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(rvar).get());
  // }
}

// ELoadArr
ELoadArr::ELoadArr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0])),
      rvar(obj::EVar::buildEVar(vec[2])) {
    vec[3].pop_back();
    offset = obj::RVar::buildRVar(vec[3].substr(1));
    // def.insert(*lvar.get());
    // use.insert(*rvar.get());
    // if (ISA(offset.get(), obj::EVar)) {
    //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(offset).get());
    // }
} 

// EBinaryExpr
EBinaryExpr::EBinaryExpr(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), lvar(obj::EVar::buildEVar(vec[0])),
      rvar1(obj::RVar::buildRVar(vec[2])),
      rvar2(obj::RVar::buildRVar(vec[4])),
      op(vec[3]) {
  // def.insert(*lvar.get());
  // if (ISA(rvar1.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(rvar1).get());
  // }
  // if (ISA(rvar2.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(rvar2).get());
  // }
}
std::string EBinaryExpr::codegen(FILE *f)  {
  auto lreg = this->f->getRegFromMem(f, lvar);
  std::string r1, r2;
  if (ISA(rvar1.get(), obj::Imm))
    r1 = std::to_string(std::dynamic_pointer_cast<obj::Imm>(rvar1)->getValue());
  else
    r1 = this->f->getRegFromMem(f, rvar1);
  if (ISA(rvar2.get(), obj::Imm))
    r2 = std::to_string(std::dynamic_pointer_cast<obj::Imm>(rvar2)->getValue());
  else
    r2 = this->f->getRegFromMem(f, rvar2);
  
  fprintf(f, "%s = %s %s %s\n", lreg.c_str(), r1.c_str(), 
                                op.c_str(), r2.c_str());

  this->f->backToMemory(f, lvar, obj::globalRegs_r[lreg]);
  // if (lreg[0] == 't' || lreg[0] == 'T' || obj::globalVars.isGlobalVar(lvar)) {
  //   this->f->backToMemory(f, lvar, obj::globalRegs_r[lreg]);
  // }
  return "";
}

// ECJump
ECJump::ECJump(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), rvar1(obj::RVar::buildRVar(vec[1])),
      rvar2(obj::RVar::buildRVar(vec[3])), lop(vec[2]), label(vec[5]) {
  // if (ISA(rvar1.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(rvar1).get());
  // }
  // if (ISA(rvar2.get(), obj::EVar)) {
  //   use.insert(*std::dynamic_pointer_cast<obj::EVar>(rvar2).get());
  // }
}


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
      // std::cerr << ptr->getVar()->getName() << " is at " << tmp << std::endl;
    }
    else if (ISA(inst.second.get(), EDefArr)) {
      std::shared_ptr<EDefArr>
          ptr = std::dynamic_pointer_cast<EDefArr>(inst.second);
      stackFrame.allocateArr(
        ptr->getVar(), ptr->getLength());
    }
  }

  // for (auto var : obj::globalVars.getAllEVars()) {
  //   stackFrame.allocateVar(var);
  // }
}

void Function::setupRegs(obj::LiveTable &liveTable) {
  regAllocator = std::make_unique<obj::RegAllocator>(liveTable, stackFrame);
  allocateResult = regAllocator->allocate();
  usedRegs = regAllocator->getUsedRegs();

  if (obj::DEBUG_FLAG) {
    // TODO: debug print
    std::cerr << "allocateResult:" << std::endl;
    for (auto entry1 : allocateResult) {
      std::cerr << "  Line " << entry1.first << ": ";
      for (auto entry2 : entry1.second) {
        std::cerr << entry2.var->getName() << "(" << entry2.regID << ") "; 
      }
      std::cerr << std::endl;
    }
  }
}

void Function::addInst(i_sptr inst) { instTable[inst->getLine()] = inst; }

i_sptr Function::getPrevInst(i_sptr inst) {
  auto it = instTable.find(inst->getLine());
  if (it == instTable.end()) {
    throw std::runtime_error("The inst of line " + std::to_string(inst->getLine())
      + " is not in function");
    return nullptr;
  }
  if (it == instTable.begin())
    return nullptr;
  -- it;
  return it->second;
}

bool Function::optimize(obj::LiveTable &liveTable) {
  // TODO: optimize
  bool opted = false;
  std::vector<int> removeList;

  for (auto &entry : instTable) {
    auto line = entry.first;
    auto inst = entry.second;
    auto liveVars = liveTable[inst->getLine()];

    if (ISA(inst.get(), EAssign)) {
      auto assign = std::dynamic_pointer_cast<EAssign>(inst);
      if (!liveVars.count(*assign->getLVar())) {
        removeList.push_back(line);
      }
    }
    if (ISA(inst.get(), ECall)) {
      auto func = std::dynamic_pointer_cast<ECall>(inst);
      if (!liveVars.count(*func->getLVar())) {
        entry.second = func->toCallVoid();
        opted = true;
      }
    }
    if (ISA(inst.get(), EUnaryExpr)) {
      auto unary = std::dynamic_pointer_cast<EUnaryExpr>(inst);
      if (!liveVars.count(*unary->getLVar())) {
        removeList.push_back(line);
      }
    }
    if (ISA(inst.get(), EBinaryExpr)) {
      auto bin = std::dynamic_pointer_cast<EBinaryExpr>(inst);
      if (!liveVars.count(*bin->getLVar())) {
        removeList.push_back(line);
      }
    }
    if (ISA(inst.get(), ELoadArr)) {
      auto load = std::dynamic_pointer_cast<ELoadArr>(inst); 
      if (!liveVars.count(*load->getLVar())) {
        removeList.push_back(line);
      }
    }
  }
  
  for (auto line : removeList) {
    opted = true;
    instTable.erase(line);
  }
  return opted;
}

void Function::codegen(FILE *f) {
  obj::VarList liveVars;

  for (auto inst : instTable) {
    // debug info
    std::string code = inst.second->getCode();
    if (code.back() == '\n') code.pop_back();
    fprintf(f, "  // line: %s -> %s\n", 
        std::to_string(inst.first).c_str(), 
        code.c_str());
    inst.second->codegen(f);

    /*
    auto allocated = allocateResult[inst.first];
    for (auto &var : allocated) {
      if (liveVars.find(*(var.var.get())) == liveVars.end()) {
        liveVars.insert(*(var.var.get()));
        loadVariable(f, inst.second->getPrevLineno() == 0, var.var, var.regID);
      }
    }

    auto spillInfos = regAllocator->getSpillInfo(inst.first + 1);
    std::for_each(spillInfos.first, spillInfos.second,
      [this, &allocated, f](auto &spillinfo) {
        auto var = spillinfo.var;
        obj::VarInfo varinfo = *allocated.find(obj::VarInfo(var, -1));
        backToMemory(f, var, varinfo.regID);
        fprintf(f, "    // spill var \n");
      });
    */
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
      // fprintf(f, "Fxxk!\n");
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

/*
std::string Function::getReg(FILE *f, rvar_sptr rvar, int lineno) {
  static int constRegUsed = 0;
  static std::string constRegs[2] = {"s11", "t5"};
  static int stkRegUsed = 0;
  static std::string stkRegs[5] = {"t0", "t1", "t2", "t3", "t4"};

  if (ISA(rvar.get(), obj::Imm)) {
    // use const reg "s11" or "t5"
    std::string reg = constRegs[constRegUsed];
    fprintf(f, "%s = x0 + %s\n", reg.c_str(), 
      std::to_string(std::dynamic_pointer_cast<obj::Imm>(rvar)->getValue()).c_str());
    constRegUsed = (constRegUsed + 1) & 1;
    return reg;
  } else {
    std::string reg;
    evar_sptr evar = std::dynamic_pointer_cast<obj::EVar>(rvar);
    auto &allocated = allocateResult[lineno];
    if (allocated.count(obj::VarInfo(evar, -2)) == 0) 
      throw std::runtime_error("Var " + evar->getName() + " is not alive");
    obj::VarInfo varinfo = *allocated.find(obj::VarInfo(evar, -2));
    if (varinfo.regID < -1)
      throw std::runtime_error("Invalid register id: " +  
                         std::to_string(varinfo.regID));
    else if (varinfo.regID == -1) {
      reg = stkRegs[stkRegUsed];
      stkRegUsed = (stkRegUsed  + 1) % 5;
      auto offset = stackFrame.addressOfVar(varinfo.var);
      stackFrame.codegen_spillout(f, offset, tmpPool.getRegID(reg));
    }
    else {
      reg = obj::globalRegs[varinfo.regID];
    }
    return reg;
  }
}
*/

std::string Function::prepareParam(FILE *f) {
  auto reg = obj::globalRegs[tempParamCount + 19];
  // _saveReg(f, tempParamCount + 19);
  tempParamCount += 1;
  return reg;
}


void Function::startFunc(FILE *f)  {
  for (int i = 0; i < paramCount; i ++) {
    int offset = 27 + i;
    stackFrame.codegen_spillin(f, offset, 19 + i);
  }
  // for (auto rid : usedRegs) {
  //   if (rid >= 1 && rid <= 12)
  //     _saveReg(f, rid);
  // }

}

/*
void Function::endFunc(FILE *f) {
  for (auto rid : usedRegs) {
    if (rid >= 1 && rid <= 12)
      _restoreReg(f, rid);
  }
}
*/

void Function::endCall(FILE *f) {
  /*
  for (auto rid : usedRegs) {
    if (rid > 19 && rid < 27) 
      _restoreReg(f, rid);
  }
  */
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
  } else {
    backToStack(f, var, reg, offreg);
  }
}

/*
void Function::loadVariable(FILE *f, bool isFirst, evar_sptr var, int reg) {
  // if (isFirst) {
  //   obj::globalVars.codegen_loadVar(f, var, obj::globalRegs[reg]);
  // }
  // else if (obj::globalVars.isGlobalVar(var)) {
  //   if (var->isArray())
  if (obj::globalVars.isGlobalVar(var)) {
      obj::globalVars.codegen_loadVar(f, var, obj::globalRegs[reg]);
  }
  else if (var->isArray()) {
    auto offset = stackFrame.addressOfVar(var);
    stackFrame.codegen_lea(f, offset, reg);
  }
}
*/

// class EInstManager
const std::string EInstManager::GLOBAL_RESMGR_NAME = "global";
const std::string EInstManager::GLOBAL_SCOPE_NAME = "f__init";

void EInstManager::addInst(const std::string &line) {
#define ADDI(type) _addInst(std::make_shared<type>(vec, curLine, getFunc(curFunc)))

  input.push_back(line);
  stringvec vec = utils::split(line, ' ');
  switch(vec.size()) {
    case 0:
      break;
    case 1:
      if(vec[0] == "return")
        ADDI(EReturn);
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

/*
std::vector<i_sptr> EInstManager::succ(InstTable &instTable, int lineno) {
  std::vector<i_sptr> ret;
  if (instTable.find(lineno) == instTable.end()) 
    return ret;
  auto inst = instTable[lineno]; 
  auto next = instTable.find(lineno);
  next ++ ;

  int line, line1, line2;

  if (ISA(inst.get(), EJump)) {
    line = getLabel(std::dynamic_pointer_cast<EJump>(inst)->getLabel());
    if (instTable.count(line))
      ret.push_back(instTable[line]);
    goto Special;
  }
  if (ISA(inst.get(), ECJump)) {
    line1 = getLabel(std::dynamic_pointer_cast<ECJump>(inst)->getLabel());
    if (instTable.count(line1))
      ret.push_back(instTable[line1]);
    if (next != instTable.end()) {
      line2 = next->second->getLine();
      if (instTable.count(line2)) 
        ret.push_back(instTable[line2]);
    }
    goto Special;
  }

  if (ISA(inst.get(), EReturn)) goto Special;
  if (ISA(inst.get(), ERetVoid)) goto Special;
  // if (ISA(inst.get(), EFunc)) goto Special;
  if (ISA(inst.get(), EEndFunc)) goto Special;
  
  if (next != instTable.end()) 
    line = next->second->getLine(); 
  if (instTable.count(line))
    ret.push_back(instTable[line]);

Special:

  return ret;
}

*/

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
      if (defvar->getVar()->getName()[0] == 'T') {
        obj::globalVars.allocateGlobalVar(f, defvar->getVar(), 0);
      }
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
    // for (auto inst : entry.second->getInsts()) 
    //   succ(entry.second->getInsts(), inst.first);

    entry.second->setupBasic();
    /*
    bool opted = false;
    obj::LiveTable liveness;
    do {
      liveness = LivenessAnalysis(*this, entry.first);
      opted = entry.second->optimize(liveness);

    } while (opted);

    entry.second->setupRegs(liveness);

    if (obj::DEBUG_FLAG) {
      std::cerr << "After Optimize: " << std::endl;
      for (auto inst : entry.second->getInsts()) {
        // std::cerr << "Line " << inst.first << ": " << inst.second->getCode() << std::endl;
        std::cerr << "[Line " << inst.first  << "] " << inst.second->getCode() 
            << std::endl << " ";
        inst.second->dbg_print();
      }
    }
    */

    entry.second->codegen(f);
  }
}

/*
struct Node {
  i_sptr inst;
  std::set<obj::EVar> in, out;
  Node(): inst(nullptr) {}
  Node(i_sptr inst) : inst(inst) {}
};

obj::LiveTable LivenessAnalysis(EInstManager &mgr, const std::string &fname) {
  using EVarSet = std::set<obj::EVar>;
  auto isSameSet = [] (EVarSet &l, EVarSet &r) {
      if (l.size() != r.size()) return false;
      for (auto i : l) if (!r.count(i)) return false;
      return true;
  };

  obj::LiveTable liveness;
  auto func = mgr.getFunc(fname);
  auto &insts = func->getInsts();

  std::map<int, Node> graph;
  for (auto entry: insts) {
    graph[entry.first] = Node(entry.second);
  }

  if (obj::DEBUG_FLAG) {
    std::cerr << "Liveness Analysis" << std::endl;
    std::cerr << " Insts:" << std::endl;
    for (auto i : insts) {
      std::cerr << "    " << i.first << ": " << i.second->getCode() << std::endl;
      std::cerr << "    --- ";
      std::cerr << "def{";
      for (auto def : i.second->getDef())
        std::cerr << def.getName() << " ";
      std::cerr << "} use{";
      for (auto use : i.second->getUse())
        std::cerr << use.getName() << " ";
      std::cerr <<"}" << std::endl;
    }
      
  }

  bool done = false;
  while (!done) {
    done = true;
    for (auto entry : insts) {
      Node &node = graph[entry.first];

      // if (obj::DEBUG_FLAG) {
      //   std::cerr << "update " << node.inst->getCode() << std::endl;
      // }
      // node.in = node.out - node.def + node.use
      EVarSet in1(node.out);
      for (auto def : node.inst->getDef())
        in1.erase(def);
      for (auto use : node.inst->getUse())
        in1.insert(use);
      
      if (!isSameSet(node.in, in1))
        done = false;
      node.in.swap(in1);

      // if (obj::DEBUG_FLAG) {
      //   std::cerr << "  new_in: ";
      //   for (auto entry : node.in)
      //     std::cerr << entry.getName() << " ";
      //   std::cerr << std::endl;
      // }

      // node.out = merge(x.in : x in succs)
      EVarSet out1;
      auto succs = mgr.succ(insts, node.inst->getLine());
      for (auto succ : succs) {
        auto succNode = graph[succ->getLine()];
        out1.insert(succNode.in.begin(), succNode.in.end());
      }

      if (!isSameSet(node.out, out1))
        done = false;
      node.out.swap(out1);

      // if (obj::DEBUG_FLAG) {
      //   std::cerr << "  new_out: ";
      //   for (auto entry : node.out)
      //     std::cerr << entry.getName() << " ";
      //   std::cerr << std::endl;
      // }
    }

    
  }
  for (auto entry : insts) {
    auto &node = graph[entry.first];
    liveness[entry.first].insert(node.out.begin(), node.out.end());
  }

  if (obj::DEBUG_FLAG) {
    std::cerr << "Finish, liveTable:" << std::endl;
    for (auto entry : liveness) {
      std::cerr << "    " << entry.first << ": ";
      for (auto entry1 : entry.second) {
        std::cerr << entry1.getName() << " ";
      }
      std::cerr << std::endl;
    }
  }
  
  return liveness;
}
*/

} // namespace ir