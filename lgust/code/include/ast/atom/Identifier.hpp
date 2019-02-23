#ifndef AST_IDENTIFIER_HPP
#define AST_IDENTIFIER_HPP

#include "ast/Node.hpp"

#include <string>

namespace langust {
namespace parse {
namespace ast {

struct Identifier : public Node {
  std::string str;

  Identifier(const std::string& s)
    : str(s) {
  }
};

} //ast
} //parse
} //langust

#endif //AST_IDENTIFIER_HPP
