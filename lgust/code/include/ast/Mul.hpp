#ifndef AST_MUL_HPP
#define AST_MUL_HPP

#include "ast/Node.hpp"

#include <vector>

namespace langust {
namespace parse {
namespace ast {

struct Mul : public Node {
  enum Type {
    MUL,
    DIV,
    MOD
  };

  std::vector<NodeObject> operands;
  std::vector<Type> operators;

  Mul(const std::vector<Type>& ops)
    : operators(ops) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_MUL_HPP
