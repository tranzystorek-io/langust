#ifndef AST_ADD_HPP
#define AST_ADD_HPP

#include "ast/Node.hpp"

#include <vector>

namespace langust {
namespace parse {
namespace ast {

struct Add : public Node {
  enum Type {
    ADD,
    SUB
  };

  std::vector<NodeObject> operands;
  std::vector<Type> operators;

  Add(const std::vector<Type>& ops)
    : operators(ops) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_ADD_HPP
