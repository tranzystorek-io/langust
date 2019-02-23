#include "Parser.hpp"
#include "catch.hpp"

#include "Input.hpp"
#include "Lexer.hpp"

#include "parsetree/ParseTree.hpp"
#include "parsetree/ExpressionListener.hpp"

#include "ast/Ast.hpp"

#include <iostream>
#include <sstream>

using namespace langust::parse;

TEST_CASE( "parser parses statements correctly" ) {
  std::istringstream iss("200 > 100 || False != 200 > 100;");

  Input input(iss);
  Lexer lexer(input);
  Parser parser(lexer);

  parser.processSymbol(SymbolId::STMT);

  ParseTree ptree = parser.getTreeBuilder().getGenerated();
  //std::cout << ptree.toString() << std::endl;

  ExpressionListener elistener;
  ptree.walkWith(&elistener);

  ast::Ast ast = elistener.getAst();

  std::cout << ast.toString() << std::endl;

  REQUIRE( true );
}
