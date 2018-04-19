#include "ParserTable.hpp"
#include "catch.hpp"

#include "Token.hpp"
#include "Symbol.hpp"

#include <algorithm>

using namespace langust::parse;

TEST_CASE( "parser table yields correct productions", "[parser_table]" ) {
  ParserTable pt;
  Symbol s;

  s = pt.getSymbol(SymbolId::EXPR, Token::Type::IDN);
  REQUIRE( s.rule.id == SymbolId::EXPR );
  REQUIRE( s.rule.index == 0 );
}

TEST_CASE( "parser table returns tokens for a given symbol", "[parser_table]" ) {
  ParserTable pt;

  using TType = Token::Type;
  ParserTable::TokenList tl;

  tl = pt.getTokensForSymbol(SymbolId::ATOM);
  std::sort(tl.begin(), tl.end());

  REQUIRE( tl == ParserTable::TokenList({TType::INT, TType::IDN, TType::BLN, TType::FUN, TType::RND_OP, TType::SQR_OP}) );
}
