#include "ProductionTable.hpp"
#include "catch.hpp"

#include "Symbol.hpp"

#include <stdexcept>
#include <algorithm>

using namespace langust::parse;

TEST_CASE( "production table yields correct productions", "[production_table]" ) {
  using PS = ProdSymbol;
  using S = SymbolId;
  using T = Token::Type;

  ProductionTable pt;

  ProductionTable::Production prod = pt.getProduction(SymbolId::F_LIT_CONT, 0);
  ProductionTable::Production expected = {PS::NonTerminal(S::PARAM_LIST),
                                          PS::Terminal(T::RND_CL),
                                          PS::Terminal(T::CUR_OP),
                                          PS::NonTerminal(S::F_LIT_CONT_A)};

  REQUIRE( std::equal(prod.begin(), prod.end(),
                      expected.begin()) );
}

TEST_CASE( "production table throws out_of_range when index doesn't exist" ) {
  ProductionTable pt;

  REQUIRE_THROWS_AS( pt.getProduction(SymbolId::STMT, 1),
                     std::out_of_range );
}
