#ifndef AST_EQ_HPP
#define AST_EQ_HPP

#include "ast/Node.hpp"

namespace langust {
namespace parse {
namespace ast {

struct Eq : public Node {
  enum Type {
    EQU,
    NEQ
  } type;

  NodeObject left;
  NodeObject right;

  Eq(Type t)
    : type(t) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_EQ_HPP
