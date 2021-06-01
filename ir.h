#ifndef _IR_H_
#define _IR_H_


#include "vars.h"
#include "regs.h"
#include "stack.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <cstring>
#include <set>
#include <map>
#include <vector>

namespace ir {
  

class EInst;

/* subclass of EInst  */
/**
 *  label:      len = 1, LABEL:
 *  retvoid:    len = 1, return 
 *  jmp:        len = 2, goto LABEL
 *  defvar:     len = 2, 'var' varmame
 *  param:      len = 2, param rvar
 *  return:     len = 2, return rvar
 *  func:       len = 2, fname '['num']'
 *  callvoid:   len = 2, call funcname
 *  endfunc:    len = 2, end funcname
 *  arrdec:     len = 3, 'var' INT varname
 *--assign:     len = 3, lvar = rvar    OPTIMIZABLE
 *--storearr:   len = 4, arr '['num']' = rval
 *  call:       len = 4, lvar = call funcname     OPTIMIZABLE
 *--unaryexpr:  len = 4, lvar = OP1 rvar    OPTIMIZABLE
 *--loadarr:    len = 4, lval = arr '['num']'    OPTIMIZABLE
 *--binaryexpr: len = 5, lvar = rvar OP2 rvar    OPTIMIZABLE
 *  cjmp:       len = 6, 'if' rvar LOP rvar 'goto' LABEL
 */
class ELable;
class ERetVoid;
class EJump;
class EDefVar;
class EParam;
class EReturn;
class EFunc;
class ECallVoid;
class EEndFunc;
class EDefArr;
class EAssign;
class EStoreArr;
class ECall;
class EUnaryExpr;
class ELoadArr;
class EBinaryExpr;
class ECJump;

class EInstManager;
class Function;


using evar_sptr = std::shared_ptr<obj::EVar>;
using rvar_sptr = std::shared_ptr<obj::RVar>;
using f_sptr = std::shared_ptr<Function>;
using i_sptr = std::shared_ptr<EInst>;
using mgr_sptr = std::shared_ptr<EInstManager>;

using stringvec = std::vector<std::string>;

class EInst {
protected:
  int lineno;
  std::string code;
  f_sptr f;
  std::set<obj::EVar> def;
  std::set<obj::EVar> use;

public:
  EInst(int lineno, f_sptr func) : lineno(lineno), f(func) {}
  virtual int getLine() { return lineno; }
  virtual std::set<obj::EVar> &getDef() { return def; }
  virtual std::set<obj::EVar> &getUse() { return use; }
  
  virtual int getPrevLineno() = 0;
  virtual i_sptr makeThis() = 0;
  virtual void setCode(const std::string &code) { this->code = code; }
  virtual std::string getCode() { return code; }
  virtual void setFunc(f_sptr f) { this->f = f; }
  virtual std::string codegen(FILE *f) = 0;
  virtual void dbg_print() const = 0;
};


class Function {

using InstTable = std::map<int, i_sptr>;

private:
  InstTable instTable;
  mgr_sptr instMgr;
  obj::RegPool tmpPool;
  obj::StackFrame stackFrame;

  std::unique_ptr<obj::RegAllocator> regAllocator;
  obj::RegAllocator::Alloc_t allocateResult;
  std::set<obj::RegID_t> usedRegs;

  int tempParamCount = 0;
  
  void _saveReg(FILE *f, int rid) {
    auto offset = stackFrame.addressOfReg(rid);
    stackFrame.codegen_spillin(f, offset, rid);
  }
  void _restoreReg(FILE *f, int rid) {
    auto offset = stackFrame.addressOfReg(rid);
    stackFrame.codegen_spillout(f, offset, rid);
  }
  
public:

  int paramCount = 0;

  Function(mgr_sptr mgr);
  
  void setupBasic();
  void setupRegs(obj::LiveTable &liveTable);

  void addInst(i_sptr inst);
  InstTable &getInsts() {
    return instTable;
  }
  obj::StackFrame &getStack() { return stackFrame; }
  i_sptr getPrevInst(i_sptr inst);
  bool optimize(obj::LiveTable &liveTable);

  
  // other methods for codegen
  void codegen(FILE *f);
  std::string getRegFromMem(FILE *f, rvar_sptr rvar, bool mustReg = 0);
  std::string getReg(FILE *f, std::shared_ptr<obj::RVar> rvar, int lineno);
  std::string prepareParam(FILE *f);
  void startFunc(FILE *f);
  void endFunc(FILE *f);
  void prepareCall(FILE *f);
  void endCall(FILE *f);
  void backToStack(FILE *f, evar_sptr lvar, int reg, int offreg = 0);
  void backToGlobal(FILE *f, evar_sptr gvar, int reg, int offreg = 0);
  void backToMemory(FILE *f, evar_sptr var, int reg, int offreg = 0);
  void loadVariable(FILE *f,  bool isFirst, evar_sptr var, int reg);
  
};


class EInstManager {
private:
  static const std::string GLOBAL_RESMGR_NAME;
  static const std::string GLOBAL_SCOPE_NAME;

  using i_sptr = std::shared_ptr<EInst>;
  using f_sptr = std::shared_ptr<Function>;
  using InstTable = std::map<int, i_sptr>;
  using FuncTable = std::map<std::string, f_sptr>;

  std::vector<std::string> input;
  InstTable instTable;
  FuncTable funcTable;
  std::map<std::string, int> labelTable;

  int curLine;
  std::string curFunc;

  i_sptr _addInst(i_sptr inst) {
    inst->setCode(input.back());
    instTable[curLine ++] = inst;
    funcTable[curFunc]->addInst(inst);
    return inst;
  }

public:

  EInstManager() {
    curLine = 1;
    curFunc = GLOBAL_SCOPE_NAME;
    funcTable[curFunc] = std::make_shared<Function>(
        std::make_shared<EInstManager>(*this));
  }
  
  void addInst(const std::string &line);
  int getLabel(const std::string &label);
  std::vector<i_sptr> succ(InstTable &instTable, int lineno);
  
  void newScope(const std::string &scopeName);
  void endScope();

  std::vector<std::string> getFuncNames() {
    std::vector<std::string> ret;
    for (auto func : funcTable) {
      ret.push_back(func.first);
    }
    return ret;
  }

  f_sptr getFunc(const std::string &name) {
    if (funcTable.find(name) == funcTable.end()) {
      throw std::runtime_error(std::string("No such function: ") + name);
    } else {
      return funcTable[name];
    }
  }

  void codegen(FILE *f);
};

obj::LiveTable LivenessAnalysis(EInstManager &mgr, const std::string &func);




/* 
 *
class ELable;
class ERetVoid;
class EJump;
class EDefVar;
class EParam;
class EReturn;
class EFunc;
class ECallVoid;
class EEndFunc;
class EDefArr;
class EAssign;
class EStoreArr;
class ECall;
class EUnaryExpr;
class ELoadArr;
class EBinaryExpr;
class ECJump;
 */
class ELable : public EInst {
private:
  std::string name;
public:
  ELable(stringvec &vec, int lineno, f_sptr f)
      :EInst(lineno, f), name(vec[0]) {}
  virtual i_sptr makeThis() { return std::make_shared<ELable>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  std::string getName() { return name; }
  virtual std::string codegen(FILE *f)  override {
    fprintf(f, "%s\n", name.c_str());
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "ELable: "  << name << std::endl;
  }
};

class ERetVoid : public EInst {
private:
public:
  ERetVoid(stringvec &, int lineno, f_sptr func) :
      EInst(lineno, func) {}
  virtual i_sptr makeThis() { return std::make_shared<ERetVoid>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    this->f->endFunc(f);
    fprintf(f, "return\n");
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "ERetVoid" << std::endl;
  }
};

class EJump : public EInst {
private:
  std::string targetLabel;
public:
  EJump(stringvec &vec, int lineno, f_sptr f) 
    : EInst(lineno, f), targetLabel(vec[1]) {}
  virtual i_sptr makeThis() { return std::make_shared<EJump>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  std::string getLabel() { return targetLabel; }
  virtual std::string codegen(FILE *f)  override {
    fprintf(f, "goto %s\n", targetLabel.c_str());
    return "";
  } 
  virtual void dbg_print() const override {
    std::cerr << "EJump : " << targetLabel << std::endl;
  }
};

class EDefVar : public EInst {
private:
  evar_sptr var;
public:
  EDefVar(stringvec &vec, int lineno, f_sptr f) 
    :EInst(lineno, f) {
    var = obj::EVar::buildEVar(vec[1]);
    def.insert(*var);
  }
  virtual i_sptr makeThis() { return std::make_shared<EDefVar>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "EDefVar : " <<  var->getName() << std::endl;
  }
  evar_sptr getVar() { return var; }
};

//TODO : constructer
class EParam : public EInst {
private:
  rvar_sptr var;
public:
  EParam(stringvec &vec, int lineno, f_sptr f); // TODO: implemented
  virtual i_sptr makeThis() { return std::make_shared<EParam>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    auto preparation = this->f->prepareParam(f);
    auto result = this->f->getRegFromMem(f, var);
    fprintf(f, "%s = %s\n", preparation.c_str(), result.c_str());
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "EParam : "  <<  var->getName() << std::endl;
  }
  
};

//TODO : constructer
class EReturn : public EInst {
private:
  rvar_sptr var;
public:
  EReturn(stringvec &vec, int lineno, f_sptr f); // TODO: to implement
  virtual i_sptr makeThis() { return std::make_shared<EReturn>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    auto reg = this->f->getRegFromMem(f, var);
    fprintf(f, "a0 = %s\n", reg.c_str());
    this->f->endFunc(f);
    fprintf(f, "return\n");
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "EReturn : " << var->getName() << std::endl;
  }

};

class EFunc : public EInst {
private:
  std::string name;
  std::string paramInfo;
public:
  EFunc(const stringvec &vec, int lineno, f_sptr f) 
    : EInst(lineno, f), name(vec[0]), paramInfo(vec[1]) {}
  virtual i_sptr makeThis() { return std::make_shared<EFunc>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  std::string getParam() { return paramInfo; }
  virtual std::string codegen(FILE *f)  override {
    auto size = this->f->getStack().size();
    fprintf(f, "%s %s [%d]\n", name.c_str(), paramInfo.c_str(), size + 1);
    this->f->startFunc(f);
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "EFunc : " << name << " " << paramInfo << std::endl;
  }
};

class ECallVoid : public EInst {
private:
  std::string name;
public:
  ECallVoid(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), name(vec[1]) {}
  virtual i_sptr makeThis() { return std::make_shared<ECallVoid>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    this->f->prepareCall(f);
    fprintf(f, "call %s\n", name.c_str());
    this->f->endCall(f);

    // auto offset = this->f->getStack().addressOfReg(19);
    // this->f->getStack().codegen_spillout(f, offset, 19);
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "ECallVoid " << name << std::endl;
  }
};

class EEndFunc : public EInst {
private:
  std::string name;
public:
  EEndFunc(stringvec &vec, int lineno, f_sptr f)
    :EInst(lineno, f), name(vec[1]) {}
  virtual i_sptr makeThis() { return std::make_shared<EEndFunc>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    fprintf(f, "end %s\n", name.c_str());
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "EEndFunc " << name << std::endl;
  }
};

//TODO : constructer
class EDefArr : public EInst {
private:
  evar_sptr var;
  int len;
public:
  EDefArr(stringvec &vec, int lineno, f_sptr f); // TODO: to implement
  evar_sptr getVar() { return var; }
  int getLength() { return len; }
  virtual i_sptr makeThis() { return std::make_shared<EDefArr>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    // TODO
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "EDefArr : " << var->getName() << " " << len
              << std::endl;
  }
};

//TODO : constructor + codegen
class EAssign : public EInst {
private:
  evar_sptr lvar;
  rvar_sptr rvar;
public:
  EAssign(const stringvec &vec, int lineno, f_sptr f); // TODO : to implement
  evar_sptr getLVar() { return lvar; }
  virtual i_sptr makeThis() { return std::make_shared<EAssign>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f) override; // TODO
  virtual void dbg_print() const override {
    std::cerr << "EAssign : " << lvar->getName() << " = "  <<rvar->getName()
              << std::endl; 
  }
}; 

//TODO : constructer
class EStoreArr : public EInst {
private:
  evar_sptr lvar;
  rvar_sptr rvar;
  rvar_sptr offset;
public:
  EStoreArr(stringvec &vec, int lineno, f_sptr f); // TODO
  virtual i_sptr makeThis() { return std::make_shared<EStoreArr>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    // dbg_print();
    auto lreg = this->f->getRegFromMem(f, lvar);
    auto rreg = this->f->getRegFromMem(f, rvar, 1);
    auto offset_reg = this->f->getRegFromMem(f, offset, 1);

    this->f->backToMemory(f, lvar, obj::globalRegs_r[rreg], 
                          obj::globalRegs_r[offset_reg]);
    return "";
  }

  virtual void dbg_print() const override {
    std::cerr << "EStoreArr: " << lvar->getName() << "[" << offset->getName()
                  << "] = " << rvar->getName() << std::endl;  
  }
};  

class ECall : public EInst {
private:
  std::string name;
  evar_sptr lvar;
public:
  ECall(stringvec &vec, int lineno, f_sptr f)
    : EInst(lineno, f), name(vec[3]) {
      lvar = obj::EVar::buildEVar(vec[0]);
      def.insert(*lvar);
    }
  virtual i_sptr makeThis() { return std::make_shared<ECall>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  evar_sptr getLVar() { return lvar; }
  i_sptr toCallVoid() {
    stringvec vec{"call", name};
    i_sptr ret = std::make_shared<ECallVoid>(vec, lineno, this->f);
    ret->setCode("call " + name);
    return ret;
  }
  virtual std::string codegen(FILE *f)  override {
    this->f->prepareCall(f);
    fprintf(f, "call %s\n", name.c_str());
    auto lreg = this->f->getRegFromMem(f, lvar);
    this->f->endCall(f);
    fprintf(f, "%s = a0\n", lreg.c_str());
    // fflush(f);
    // auto offset = this->f->getStack().addressOfReg(19);
    // this->f->getStack().codegen_spillout(f, offset, 19);
    // fflush(f);
    if (lreg[0] == 't'|| lreg[0] == 'T'  || obj::globalVars.isGlobalVar(lvar)) {
      this->f->backToMemory(f, lvar, obj::globalRegs_r[lreg]);
    }
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "ECall : " << name << " -> " << lvar->getName() << std::endl;
  }
};

//TODO : constructer
class EUnaryExpr : public EInst {
private:
  evar_sptr lvar;
  std::string op;
  rvar_sptr rvar;
public:
  EUnaryExpr(stringvec &vec, int lineno, f_sptr f); 
  virtual i_sptr makeThis() { return std::make_shared<EUnaryExpr>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  evar_sptr getLVar() { return lvar; }
  virtual std::string codegen(FILE *f)  override {
    auto lreg = this->f->getRegFromMem(f, lvar);
    auto rreg = this->f->getRegFromMem(f, rvar);
    fprintf(f, "%s = %s %s\n", lreg.c_str(), op.c_str(), rreg.c_str());

    if (lreg[0] == 't'|| lreg[0] == 'T'  || obj::globalVars.isGlobalVar(lvar)) {
      this->f->backToMemory(f, lvar, obj::globalRegs_r[lreg]);
    }
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "EUnaryExpr : " << lvar->getName() << " = " <<
                 op << " " << rvar->getName() << std::endl;
  }
};

//TODO : constructer
class ELoadArr : public EInst {
private:
  evar_sptr lvar;
  rvar_sptr offset;
  evar_sptr rvar; 
public:
  ELoadArr(stringvec &vec, int lineno, f_sptr f); 
  evar_sptr getLVar() { return lvar; }
  virtual i_sptr makeThis() { return std::make_shared<ELoadArr>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  virtual std::string codegen(FILE *f)  override {
    auto lreg = this->f->getRegFromMem(f, lvar);
    auto rreg = this->f->getRegFromMem(f, rvar);
    auto offset_reg = this->f->getRegFromMem(f, offset, 1);

    fprintf(f, "%s = %s + %s\n", lreg.c_str(), rreg.c_str(), offset_reg.c_str());
    fprintf(f, "%s = %s [0]\n", lreg.c_str(), lreg.c_str());
    if (lreg[0] == 't' || lreg[0] == 'T' || obj::globalVars.isGlobalVar(lvar)) {
      this->f->backToMemory(f, lvar, obj::globalRegs_r[lreg]);
    }
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "ELoadArr : " << lvar->getName() << " = " << rvar->getName()
                 << "[" << offset->getName() << "]" << std::endl;
  }
};

//TODO : constructer + codegen
class EBinaryExpr : public EInst {
private:
  evar_sptr lvar;
  rvar_sptr rvar1, rvar2;
  std::string op;
public:
  EBinaryExpr(stringvec &vec, int lineno, f_sptr f);  
  virtual i_sptr makeThis() { return std::make_shared<EBinaryExpr>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  evar_sptr getLVar() { return lvar; }
  virtual std::string codegen(FILE *f)  override;
  virtual void dbg_print() const override {
    std::cerr << "EBinaryExpr : " << lvar->getName() << " = " 
              << rvar1->getName() << op << rvar2->getName() << std::endl;
  }
};

//TODO : constructer
class ECJump : public EInst {
private:
  rvar_sptr rvar1, rvar2;
  std::string lop;
  std::string label;

public:
  ECJump(stringvec &vec, int lineno, f_sptr f);
  virtual i_sptr makeThis() { return std::make_shared<ECJump>(*this);}
  virtual int getPrevLineno() {
    i_sptr current = makeThis();
    i_sptr prev = f->getPrevInst(current);
    if (prev) {
      return prev->getLine();
    } else
      return 0;
  }
  std::string getLabel() { return label; }
  virtual std::string codegen(FILE *f)  override {
    auto reg1 = this->f->getRegFromMem(f, rvar1);
    auto reg2 = this->f->getRegFromMem(f, rvar2, 1);
    fprintf(f, "if %s %s %s goto %s\n", reg1.c_str(), lop.c_str()
                                      , reg2.c_str(), label.c_str());
    return "";
  }
  virtual void dbg_print() const override {
    std::cerr << "ECJump : " << rvar1->getName() << lop << rvar2->getName()
              << " -> " << label << std::endl;
  }
};

}  // namespace IR
#endif // _IR_H_