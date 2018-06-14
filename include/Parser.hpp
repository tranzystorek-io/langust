#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "ProductionTable.hpp"
#include "parsetree/ParseTreeBuilder.hpp"

#include <stack>

namespace langust {
namespace parse {

class Parser {
public:
  using Stack = std::stack<ProdSymbol>;

public:
  // Parser(std::istream& in, const Lexer::Callback& callback = Lexer::Callback());
  Parser(Lexer& lexer);

  bool processSymbol(SymbolId sym);
  ParseTreeBuilder& getTreeBuilder();

  void reset();
  bool isReady() const;

private:
  void pushProduction(const ProductionTable::Production& prod);
  void reportUnexpectedToken(const Token& t, ProdSymbol ps) const;

private:
  Stack stack_;
  ParseTreeBuilder treebuilder_;

  Lexer& lexer_;
};

}
}

#endif //PARSER_HPP
