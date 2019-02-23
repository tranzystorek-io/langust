#ifndef AST_FUNCLITERAL_HPP
#define AST_FUNCLITERAL_HPP

#include "ast/Node.hpp"

#include <vector>
#include <string>

namespace langust {
namespace parse {
namespace ast {

struct FuncLiteral : public Node {
  std::vector<std::string> params;
  std::vector<NodeObject> ops;
  NodeObject ret;

  FuncLiteral(const std::vector<std::string>& p)
    : params(p) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_FUNCLITERAL_HPP
