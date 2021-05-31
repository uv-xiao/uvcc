#ifndef _TYPES_H
#define _TYPES_H

#include <string>
#include <set> 
#include <map>
#include <memory>

namespace obj
{
  
extern bool DEBUG_FLAG;

class RVar;
class Imm;
class EVar;
class TVar;
class VarPool;
class GlobalVarManager;

using VarList = std::set<EVar>;
using LiveTable = std::map<int, VarList>;
using rvar_sptr = std::shared_ptr<RVar>;
using evar_sptr = std::shared_ptr<EVar>;
using tvar_sptr = std::shared_ptr<TVar>;
using Reg = std::string;
using RegID_t = int;
  
} // namespace obj



#endif