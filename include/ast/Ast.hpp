#ifndef AST_AST_HPP
#define AST_AST_HPP

#include "ast/Node.hpp"

namespace langust {
namespace parse {
namespace ast {

class Ast {
public:
  Ast(Node* root, NodeObject::Type type)
    : root_(root, type) {
  }

  std::string toString() const;

private:
  NodeObject root_;
};

} //ast
} //parse
} //langust

#endif //AST_AST_HPP
