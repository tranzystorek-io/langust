#ifndef ASSIGN_HPP
#define ASSIGN_HPP

#include "ast/Node.hpp"

namespace langust {
namespace parse {
namespace ast {

struct Assign : public Node {
  NodeObject lhs;
  NodeObject rhs;
};

} //ast
} //parse
} //langust

#endif //ASSIGN_HPP
