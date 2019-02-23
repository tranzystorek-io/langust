#ifndef AST_AND_HPP
#define AST_AND_HPP

#include "ast/Node.hpp"

#include <vector>

namespace langust {
namespace parse {
namespace ast {

struct And : public Node {
  std::vector<NodeObject> operands;
};

} //ast
} //parse
} //langust

#endif //AST_AND_HPP
