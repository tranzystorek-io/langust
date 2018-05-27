#ifndef AST_REL_HPP
#define AST_REL_HPP

#include "ast/Node.hpp"

namespace langust {
namespace parse {
namespace ast {

struct Rel : public Node {
  enum Type {
    LT, LE,
    GT, GE
  } type;
  NodeObject left;
  NodeObject right;

  Rel(Type t)
    : type(t) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_REL_HPP
