#ifndef LANGUST_MEMORY_ARENA_HPP
#define LANGUST_MEMORY_ARENA_HPP

#include "ast/atom/FuncLiteral.hpp"

#include <memory>
#include <string>
#include <list>
#include <unordered_map>
#include <stack>

namespace langust {
namespace interpreter {

enum VarType {
  INT,
  BOOL,
  LIST,
  FUNC,
  VOID
};

struct Object {
  using List = std::list<Object>;
  using FuncPtr = std::shared_ptr<parse::ast::Node>;

  VarType vtype;

  union {
    int i;
    bool b;
  };

  std::shared_ptr<List> list;
  FuncPtr func;

  Object()
    : vtype(VarType::VOID) {
  }

  explicit Object(int intval)
    : vtype(VarType::INT),
      i(intval) {
  }

  explicit Object(bool boolval)
    : vtype(VarType::BOOL),
      b(boolval) {
  }

  explicit Object(List* listptr)
    : vtype(VarType::LIST),
      list(listptr) {
  }

  Object(const FuncPtr& funcptr)
    : vtype(VarType::FUNC),
      func(funcptr) {
  }

  bool operator==(const Object& other) const;
  bool operator!=(const Object& other) const;
};

class MemoryArena {
public:
  using Variable = std::shared_ptr<Object>;
  using VarSpace = std::unordered_map<std::string, Variable>;
  using Stack = std::stack<VarSpace>;

public:
  bool isDefined(const std::string& varname) const;
  bool isStackEmpty() const;

  Variable& at(const std::string& varname);
  void put(const std::string& varname, const Variable& var);

  void enterFrame();
  void exitFrame();

private:
  VarSpace global_;
  Stack stack_;
};

} //interpreter
} //langust

#endif //LANGUST_MEMORY_ARENA_HPP
