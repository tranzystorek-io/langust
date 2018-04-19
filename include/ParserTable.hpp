#ifndef PARSERTABLE_HPP
#define PARSERTABLE_HPP

#include <unordered_map>
#include <vector>

#include "Token.hpp"
#include "Symbol.hpp"

namespace langust {
namespace parse {

class ParserTable {
public:
  using Row = std::unordered_map<Token::Type, Symbol>;
  using Table = std::unordered_map<SymbolId, Row>;

  using TokenList = std::vector<Token::Type>;

public:
  ParserTable();

  Symbol getSymbol(SymbolId id, Token::Type t) const;
  TokenList getTokensForSymbol(SymbolId id) const;

private:
  void initTable();

private:
  Table table_;
};

} //parse
} //langust

#endif //PARSERTABLE_HPP
