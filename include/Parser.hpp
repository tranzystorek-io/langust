#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "ParserTable.hpp"
#include "ProductionTable.hpp"

#include <stack>

namespace langust {
namespace parse {

class Parser {
public:
  using Stack = std::stack<ProdSymbol>;

public:
  Parser(std::istream& in);

  void processSymbol(SymbolId sym);

private:
  void pushProduction(const ProductionTable::Production& prod);
  void reportUnexpectedToken(const Token& t, ProdSymbol ps) const;

private:
  Stack stack_;
  ParserTable partable_;
  ProductionTable prodtable_;

  Lexer lexer_;
};

}
}

#endif //PARSER_HPP
