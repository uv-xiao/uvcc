#include "riscv.h"
#include <assert.h>

namespace riscv {

int DEBUG_FLAG = 0;

stringset reg_names = {"x0", "t0", "t1", "t2", "t3", "t4", "t5", "t6", 
                      "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
                      "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", 
                      "s8", "s9", "s10", "s11", "s12"};
stringmap expr_ops = {{"+", "add"}, {"-", "sub"},
                      {"*", "mul"}, {"/", "div"}, {"%", "rem"},
                      {"<", "slt"}, {">", "sgt"}},
          seqz_ops = {{">=", "slt"}, {"<=", "sgt"}, {"==", "xor"}},
          snez_ops = {{"||", "or"}, {"!=", "xor"}},
          goto_ops = {{"<", "blt"}, {">", "bgt"}, {"!=", "bne"},
                      {"==","beq"}, {"<=","ble"}, {">=", "bge"}},
          rimm_ops = {{"+", "addi"}, {"<", "slti"}};
std::string emptyReg = "s0";

stringvec RISCVGen::_split(const std::string &line, char sep) {
    std::stringstream sin(line);
    std::vector<std::string> ret;
    std::string part;
    char ch;
    while (sin.get(ch)) {
      if ((ch == '[')) {
        if (part.length() > 0)
          ret.push_back(part);
        part = "[";
        while (sin.get(ch) && ch != ']') {
          if (ch != ' ')
            part = part + ch;
        }
        assert(ch == ']');
        part = part + ch;
      }
      else if ((ch == sep || ch == '\n')) {
        if (part.length() > 0)
          ret.push_back(part);
        part = "";
      }
      else if (ch == '\r')
        continue;
      else {
        part = part + ch;
      }      
    }
    if (part.length()) {
      ret.push_back(part);
    }
    return ret;
}
RISCVGen::TType RISCVGen::_which(const stringvec &line) {
  const int len = line.size();
  if(len == 0) return TType::Empty;

  /* f_name [pcnt] [size] */
  if(len == 3 && _substr(line[0], "f_"))
    return TType::TFunc;

  /* end f_name */
  if(len == 2 && line[0] == "end")
    return TType::TEndFunc;

  /* v1 = malloc 12345 */
  if(len == 4 && line[2] == "malloc")
    return TType::TGlobalArr;

  /* v0 = 0 */
  if(len == 3 && ! _inSet(reg_names, line[0]) && 
                  _isNum(line[2]) && line[1] == "=")
    return TType::TGlobalVar;

  /* reg = integer */
  if(len == 3 && _inSet(reg_names, line[0]) && _isNum(line[2]))
    return TType::TAssignInt;

  /* reg = reg +/< integer */  
  if (len == 5 && _inSet(reg_names, line[0])
          && _inSet(reg_names, line[2]) && _isNum(line[4]))
    return TType::TBiImm;

  /* reg1 = reg2 op reg3 */
  if(len == 5 && _inSet(reg_names, line[0]) 
          && _inSet(reg_names, line[2]) && _inSet(reg_names, line[4])
          && _inMap(expr_ops, line[3]))
    return TType::TBiExpr;

  /* reg1 = reg2 >=/<=/== reg3 */
  if(len == 5 && _inSet(reg_names, line[0]) 
          && _inSet(reg_names, line[2]) && _inSet(reg_names, line[4])
          && _inMap(seqz_ops, line[3]))
    return TType::TBiseqz;

  /* reg1 = reg2 ||/!= reg3*/
  if(len == 5 && _inSet(reg_names, line[0]) 
          && _inSet(reg_names, line[2]) && _inSet(reg_names, line[4])
          && _inMap(snez_ops, line[3]))
    return TType::TBisnez;

  /* reg1 = reg2 && reg3 */
  if(len == 5 && _inSet(reg_names, line[0]) 
          && _inSet(reg_names, line[2]) && _inSet(reg_names, line[4])
          && line[3] == "&&") // need jmp
    return TType::TAnd;

  /* reg1 = reg2 */
  if(len == 3 && _inSet(reg_names, line[0]) && _inSet(reg_names, line[2]))
    return TType::TAssignReg;

  /* reg1 = [-/!]reg2 */
  if (len == 3 && _inSet(reg_names, line[0]) 
               && _inSet(reg_names, line[2].substr(1)))
    return TType::TUExpr3;

  /* reg1 = -/! reg2 */
  if(len == 4 && line[1] == "=" && (line[2] == "-" || line[2] == "!") && 
      _inSet(reg_names, line[0]) && _inSet(reg_names, line[3]))
    return TType::TUExpr4;

  /* reg [1] = reg */
  if(len == 4 && line[2] == "=" && line[1].back() == ']')
    return TType::TAssignToArr;

  /* reg = reg [2] */
  if(len == 4 && line[1] == "=" && line[3].back() == ']')
    return TType::TAssignFromArr;

  /* 0  1    2  3    4    5  */
  /* if reg1 op reg2 goto l3 */
  if(len == 6 && line[4] == "goto")
    return TType::TCJump;

  /* label */
  if(len == 1 && line[0].back() == ':')
    return TType::TLabel;

  /* goto Label */
  if(len == 2 && line[0] == "goto")
    return TType::TJump;

  /* call function */
  if(len == 2 && line[0] == "call")
    return TType::TCall;

  /* store r0 4 */
  if(len == 3 && line[0] == "store")
    return TType::TStore;

  /* load int reg */
  if(len == 3 && line[0] == "load")
    return TType::TLoad;

  /* loadaddr int/gvar reg */
  if(len == 3 && line[0] == "loadaddr")
    return TType::TLoadAddr;

  /* return */
  if(line[0] == "return")
    return TType::TReturn;
  
  throw std::runtime_error("Unknown in _which");
  return TType::Empty;
}

void RISCVGen::_codegen(FILE *f, stringvec line) {
  if (DEBUG_FLAG) {
    std::cerr << "{" << std::endl;
    for (auto s : line) {
      std::cerr << s << std::endl;
    }
    std::cerr << "}" << std::endl;
  }

  switch (_which(line)) {
    case TType::Empty :
      break;
    case TType::TFunc : {            /* f_name [pcnt] [size] */
      int size = std::stoi(line[2].substr(0,line[2].length() - 1).substr(1));
      std::string fname = line[0].substr(2);
      Emit(f, "\t.text");
      Emit(f, "\t.align\t2");
      Emit(f, "\t.global\t" + fname);
      Emit(f, "\t.type\t" + fname + ", @function");
      Emit(f, fname + ":");
      stackSize = (size / 4 + 1) * 16;
      Emit(f, "\tadd\tsp, sp, -" + std::to_string(stackSize));
      Emit(f, "\tsw\tra, " +std::to_string(stackSize - 4) + "(sp)");  
      break;
    }    
    case TType::TEndFunc : {        /* end f_name */
      auto name = line[1].substr(2);
      Emit(f, "\t.size\t" + name + ", .-" + name);
      break;
    }
    case TType::TGlobalArr : {      /* v1 = malloc 12345 */
      std::string size = std::to_string(std::stoi(line[3]));
      Emit(f, std::string("\t.comm\t") + line[0] + ", " + size + ", 4");
      break;
    }
    case TType::TGlobalVar : {      /* v0 = 0 */
      auto &vname = line[0];
      Emit(f, "\t.global\t" + vname);
      Emit(f, "\t.section\t.sdata");
      Emit(f, "\t.align\t2");
      Emit(f, "\t.type\t" + vname + ", @object");
      Emit(f, "\t.size\t" + vname + ", 4");
      Emit(f, vname + ":");
      Emit(f, "\t.word\t" + line[2]);
      break;
    }
    case TType::TAssignInt : {      /* reg = integer */
      Emit(f, "\tli\t" + line[0] + ", " + line[2]);
      break;
    }
    case TType::TBiExpr : {         /* reg1 = reg2 op reg3 */
      Emit(f, "\t" + expr_ops[line[3]] + "\t" + line[0] + ", " + line[2] + ", " + line[4]);
      break;
    }
    case TType::TBiseqz : {         /* reg1 = reg2 seqz_op reg3 */
      Emit(f, "\t" + seqz_ops[line[3]] + "\t" + line[0] + ", " + line[2] + ", " + line[4]);
      Emit(f, "\tseqz\t"  + line[0] + ", " + line[0]);
      break;
    }
    case TType::TBisnez : {         /* reg1 = reg2 snez_op reg3 */
      Emit(f, "\t" + snez_ops[line[3]] + "\t" + line[0] + ", " + line[2] + ", " + line[4]);
      Emit(f, "\tsnez\t"   + line[0] + ", " + line[0]);
      break;
    }
    case TType::TAnd : {            /* reg1 = reg2 && reg3 */
      Emit(f, std::string("\tsnez") + "\t" + line[0] + ", " + line[2]);
      Emit(f, std::string("\tsnez") + "\t" + emptyReg + ", " + line[4]);
      Emit(f, std::string("\tand\t") + line[0] + ", " + line[0] + ", " + emptyReg);
      break;
    }
    case TType::TBiImm : {          /* reg1 = reg2 op int*/

      if (DEBUG_FLAG)
        std::cerr << "Meet TBiImm!" << std::endl;
      Emit(f, std::string("\tli") + "\t" + emptyReg + ", " + line[4]);
      line[4] = emptyReg;
      _codegen(f, line);
      break;
    }
    case TType::TAssignReg : {      /* reg1 = reg2 */
      Emit(f, "\tmv\t" + line[0] + ", " + line[2]);
      break;
    }
    case TType::TUExpr3 : {         /* reg1 = [-/!]reg2 */
      Emit(f, ((line[2][0] == '-') ? "\tneg\t" : "\tseqz\t") + 
              line[0] + ", " + line[2].substr(1));
      break;
    }
    case TType::TUExpr4 : {         /* reg1 = -/! reg2 */
      Emit(f, ((line[2][0] == '-') ? "\tneg\t" : "\tseqz\t") + 
              line[0] + ", " + line[3]);
      break;
    }
    case TType::TAssignToArr : {    /* reg [1] = reg */
      auto snum = line[1].substr(0, line[1].length() - 1).substr(1);
      Emit(f, "\tsw\t" + line[3] + ", " + snum + "(" + line[0] + ")");
      break;
    }
    case TType::TAssignFromArr : {  /* reg = reg [2] */
      auto snum = line[3].substr(0, line[3].length() - 1).substr(1);
      Emit(f, "\tlw\t" + line[0] + ", " + snum + "(" + line[2] + ")");
      break;
    }
    case TType::TCJump : {          /* if reg1 op reg2 goto l3 */
      Emit(f, "\t" + goto_ops[line[2]] + "\t" + line[1] + ", " + line[3] 
          + ", ." + line[5]);
      break;
    }
    case TType::TLabel : {          /* label */
      Emit(f, "." + line[0]);
      break;
    }
    case TType::TJump : {           /* goto Label */
      Emit(f, "\tj\t." + line[1]);
      break;
    }
    case TType::TCall : {           /* call function */
      auto fname = line[1].substr(2);
      Emit(f, "\tcall\t"+fname);
      break;
    }
    case TType::TStore : {          /* store r0 4 */
      int num = std::stoi(line[2]) * 4;
      std::string strnum;
      if (num > 2047 || num < -2048) {
        Emit(f, "\tli\t" + emptyReg + ", " + std::to_string(num));
        strnum = emptyReg;
      }
      else strnum = std::to_string(num);
      Emit(f, "\tsw\t" + line[1] + ", " + strnum + "(sp)");
      break;
    }
    case TType::TLoad : {           /* load int reg */
      if(_isNum(line[1])) {
        int num = 4 * std::stoi(line[1]);
        std::string strnum;
        if (num > 2047 || num < -2048) {
          Emit(f, "\tli\t" + emptyReg + ", " + std::to_string(num));
          strnum = emptyReg;
        }
        else strnum = std::to_string(num);
        Emit(f, "\tlw\t" + line[2] + ", " + strnum + "(sp)");
      }
      else {
        Emit(f, "\tlui\t" + line[2] + ", \%hi(" + line[1] + ")");
        Emit(f, "\tlw\t" + line[2] + 
             ", " + "\%lo(" + line[1] + ")(" + line[2] + ")");
      }
      break;
    }
    case TType::TLoadAddr : {       /* loadaddr int/gvar reg */
      if(_isNum(line[1])) { 
        int num = 4 * std::stoi(line[1]);
        std::string strnum;
        if (num > 2047 || num < -2048) {
          Emit(f, "\tli\t" + emptyReg + ", " + std::to_string(num));
          strnum = emptyReg;
        }
        else strnum = std::to_string(num);
        Emit(f, "\tadd\t" + line[2] + ", sp, " + strnum);
      }
      else 
        Emit(f, "\tla\t" + line[2] + ", " + line[1]);
      break;
    }
    case TType::TReturn : {         /* return */
      Emit(f, "\tlw\tra, " + std::to_string(stackSize - 4) + "(sp)");
      Emit(f, "\taddi\tsp, sp, " + std::to_string(stackSize));
      Emit(f, "\tret");
      break;
    }
    default:
      throw std::runtime_error("Not recognized line");
      break;
  }
}

} // namespace riscv