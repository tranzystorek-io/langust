#ifndef AST_NOT_HPP
#define AST_NOT_HPP

#include "ast/Node.hpp"

#include <memory>

namespace langust {
namespace parse {
namespace ast {

struct Not : public Node {
  NodeObject operand;
};

} //ast
} //parse
} //langust

#endif //AST_NOT_HPP
