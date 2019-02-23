#ifndef AST_OR_HPP
#define AST_OR_HPP

#include "ast/Node.hpp"

#include <vector>

namespace langust {
namespace parse {
namespace ast {

struct Or : public Node {
  std::vector<NodeObject> operands;
};

} //ast
} //parse
} //langust

#endif //AST_OR_HPP
