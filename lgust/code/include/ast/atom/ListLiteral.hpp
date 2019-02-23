#ifndef AST_LISTLITERAL_HPP
#define AST_LISTLITERAL_HPP

#include "ast/Node.hpp"

#include <vector>

namespace langust {
namespace parse {
namespace ast {

struct ListLiteral : public Node {
  std::vector<NodeObject> exprs;
};

} //ast
} //parse
} //langust

#endif //AST_LISTLITERAL_HPP
