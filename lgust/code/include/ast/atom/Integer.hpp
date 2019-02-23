#ifndef AST_INTEGER_HPP
#define AST_INTEGER_HPP

#include "ast/Node.hpp"

namespace langust {
namespace parse {
namespace ast {

struct Integer : public Node {
  int value;

  Integer(int v = 0)
    : value(v) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_INTEGER_HPP
