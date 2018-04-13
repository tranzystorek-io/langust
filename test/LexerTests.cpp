#include "Lexer.hpp"
#include "catch.hpp"

#include <sstream>

using namespace langust::parse;

TEST_CASE( "tokens are scanned", "[lexer]" ) {
  using TokType = langust::parse::Token::Type;

  std::istringstream iss;
  Token t = Token::Unknown();
  Lexer lexer(iss);

  SECTION( "identifiers" ) {
    iss.str("a a1 a_ _ _a _1 _a_ _1_ 1a");

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "a" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "a1" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "a_" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "_" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "_a" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "_1" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "_a_" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "_1_" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::INT );
    REQUIRE( t.str == "1" );
    REQUIRE( t.value.i == 1 );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::IDN );
    REQUIRE( t.str == "a" );
  } // identifiers

  SECTION( "negative integers" ) {
    iss.str("-0 -1");

    t = lexer.getToken();
    REQUIRE( t.type == TokType::SUB );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::INT );
    REQUIRE( t.str == "0" );
    REQUIRE( t.value.i == 0 );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::INT );
    REQUIRE( t.str == "-1" );
    REQUIRE( t.value.i == -1 );
  } //negative integers

  SECTION( "special strings" ) {
    iss.str("func return True False");

    t = lexer.getToken();
    REQUIRE( t.type == TokType::FUN );
    REQUIRE( t.str == "func" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::RET );
    REQUIRE( t.str == "return" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::BLN );
    REQUIRE( t.str == "True" );
    REQUIRE( t.value.b == true );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::BLN );
    REQUIRE( t.str == "False" );
    REQUIRE( t.value.b == false );
  } // special strings

  SECTION( "operators" ) {
    iss.str("> >= >? >: < <= && || = == ! != + - * / %");

    t = lexer.getToken();
    REQUIRE( t.type == TokType::GT );
    REQUIRE( t.str == ">" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::GE );
    REQUIRE( t.str == ">=" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::FLT );
    REQUIRE( t.str == ">?" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::MAP );
    REQUIRE( t.str == ">:" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::LT );
    REQUIRE( t.str == "<" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::LE );
    REQUIRE( t.str == "<=" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::AND );
    REQUIRE( t.str == "&&" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::OR );
    REQUIRE( t.str == "||" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::ASN );
    REQUIRE( t.str == "=" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::EQU );
    REQUIRE( t.str == "==" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::NOT );
    REQUIRE( t.str == "!" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::NEQ );
    REQUIRE( t.str == "!=" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::ADD );
    REQUIRE( t.str == "+" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::SUB );
    REQUIRE( t.str == "-" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::MUL );
    REQUIRE( t.str == "*" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::DIV );
    REQUIRE( t.str == "/" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::MOD );
    REQUIRE( t.str == "%" );
  } // operators

  SECTION( "other" ) {
    iss.str("{} () [] . , : ;");

    t = lexer.getToken();
    REQUIRE( t.type == TokType::CUR_OP );
    REQUIRE( t.str == "{" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::CUR_CL );
    REQUIRE( t.str == "}" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::RND_OP );
    REQUIRE( t.str == "(" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::RND_CL );
    REQUIRE( t.str == ")" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::SQR_OP );
    REQUIRE( t.str == "[" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::SQR_CL );
    REQUIRE( t.str == "]" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::DOT );
    REQUIRE( t.str == "." );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::CMM );
    REQUIRE( t.str == "," );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::COL );
    REQUIRE( t.str == ":" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::SCL );
    REQUIRE( t.str == ";" );
  } // other

  SECTION( "invalid symbols" ) {
    iss.str("$ @ # ^ ' \" | & \\ ? ` ~");

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "$" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "@" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "#" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "^" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "'" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "\"" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "|" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "&" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "\\" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "?" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "`" );

    t = lexer.getToken();
    REQUIRE( t.type == TokType::UNKNOWN );
    REQUIRE( t.str == "~" );
  } // invalid symbols
}

TEST_CASE( "exception is thrown when identifier too long", "[lexer]" ) {
  std::string s(LANGUST_MAX_IDENTIFIER_LENGTH + 1, 'a');
  std::istringstream iss(s);

  Lexer lexer(iss);

  REQUIRE_THROWS( lexer.getToken() );
}

TEST_CASE( "file position is tracked", "[lexer]" ) {
  std::istringstream iss("ab 12\n"
                         "ef >:");

  Lexer lexer(iss);
  Token t = Token::Unknown();

  t = lexer.getToken();
  REQUIRE( t.pos.line == 1 );
  REQUIRE( t.pos.col == 0 );

  t = lexer.getToken();
  REQUIRE( t.pos.line == 1 );
  REQUIRE( t.pos.col == 3 );

  t = lexer.getToken();
  REQUIRE( t.pos.line == 2 );
  REQUIRE( t.pos.col == 0 );

  t = lexer.getToken();
  REQUIRE( t.pos.line == 2 );
  REQUIRE( t.pos.col == 3 );
}
