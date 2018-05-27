#ifndef AST_LIST_HPP
#define AST_LIST_HPP

#include "ast/Node.hpp"

#include <vector>

namespace langust {
namespace parse {
namespace ast {

struct List : public Node {
  enum Type {
    FLT,
    MAP,
    //List Access
    ACCESS_ONE,
    ACCESS_FROM,
    ACCESS_TO,
    ACCESS_RANGE
  };

  NodeObject initial;
  std::vector<NodeObject> operands;
  std::vector<Type> operators;

  List(const std::vector<Type>& ops)
    : operators(ops) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_LIST_HPP
