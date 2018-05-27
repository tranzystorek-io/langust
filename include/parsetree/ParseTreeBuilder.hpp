#ifndef PARSETREE_BUILDER_HPP
#define PARSETREE_BUILDER_HPP

#include "Token.hpp"
#include "Symbol.hpp"
#include "parsetree/ParseTree.hpp"

#include <stack>

namespace langust {
namespace parse {

struct TreeLocation {
  TreeLocation(Node* ptr, int psize)
    : ptr(ptr),
      prodsize(psize) {
  }

  Node* ptr;
  size_t prodsize;
};

class ParseTreeBuilder {
public:
  using Stack = std::stack<TreeLocation>;
public:
  ParseTree&& getGenerated();

  void addNode(const Token& t);
  void addNode(SymbolId id, int prod);

private:
  void unwindStack();

private:
  ParseTree generated_;
  Stack stack_;
};

} //parse
} //langust

#endif //PARSETREE_BUILDER_HPP
