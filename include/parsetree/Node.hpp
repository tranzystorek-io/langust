#ifndef PARSETREE_NODE_HPP
#define PARSETREE_NODE_HPP

#include "Symbol.hpp"
#include "Token.hpp"

#include <vector>
#include <memory>

namespace langust {
namespace parse {

struct Rule {
  SymbolId id;
  int production;
};

struct Node {
  using Ptr = std::unique_ptr<Node>;

  enum Type {
    TERMINAL,
    NONTERMINAL
  } type;

  struct {
    Token tok;
    Rule rule;
  } value;

  std::vector<Ptr> children;

  Node(const Token& t)
    : type(Type::TERMINAL) {
    value.tok = t;
  }

  Node(SymbolId id, int prod)
    : type(Type::NONTERMINAL) {
    value.rule.id = id;
    value.rule.production = prod;
  }
};

} //parse
} //langust

#endif //PARSETREE_NODE_HPP
