#ifndef AST_FUNCAPPLY_HPP
#define AST_FUNCAPPLY_HPP

#include "ast/Node.hpp"

#include <vector>
#include <string>

namespace langust {
namespace parse {
namespace ast {

struct FuncApply : public Node {
  std::string name;
  std::vector<NodeObject> exprs;

  FuncApply(const std::string& n)
    : name(n) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_FUNCAPPLY_HPP
