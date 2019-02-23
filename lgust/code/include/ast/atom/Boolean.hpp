#ifndef AST_BOOLEAN_HPP
#define AST_BOOLEAN_HPP

#include "ast/Node.hpp"

namespace langust {
namespace parse {
namespace ast {

struct Boolean : public Node {
  bool value;

  Boolean(bool v = false)
    : value(v) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_BOOLEAN_HPP
