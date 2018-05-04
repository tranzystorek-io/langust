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

  SECTION( "integers" ) {
    SECTION( "positive" ) {
      iss.str("128");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::INT );
      REQUIRE( t.str == "128" );
      REQUIRE( t.value.i == 128 );
    }

    SECTION( "negative" ) {
      iss.str("-512");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::INT );
      REQUIRE( t.str == "-512" );
      REQUIRE( t.value.i == -512 );
    }

    SECTION( "zero" ) {
      iss.str("0");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::INT );
      REQUIRE( t.str == "0" );
      REQUIRE( t.value.i == 0 );
    }

    SECTION( "\"minus zero\" corner case" ) {
      iss.str("-0");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::ADD_OP );
      REQUIRE( t.str == "-" );
      REQUIRE( t.value.add_t == AddOpType::SUB );

      t = lexer.getToken();
      REQUIRE( t.type == TokType::INT );
      REQUIRE( t.str == "0" );
      REQUIRE( t.value.i == 0 );
    }
  } //integers

  SECTION( "special strings" ) {
    iss.str("func return True False");

    SECTION( "func" ) {
      iss.str("func");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::FUN );
      REQUIRE( t.str == "func" );
    }

    SECTION( "return" ) {
      iss.str("return");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::RET );
      REQUIRE( t.str == "return" );
    }

    SECTION( "True" ) {
      iss.str("True");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::BLN );
      REQUIRE( t.str == "True" );
      REQUIRE( t.value.b == true );
    }

    SECTION( "False" ) {
      iss.str("False");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::BLN );
      REQUIRE( t.str == "False" );
      REQUIRE( t.value.b == false );
    }
  } // special strings

  SECTION( "operators" ) {
    SECTION( "greater than" ) {
      iss.str(">");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::REL_OP );
      REQUIRE( t.str == ">" );
      REQUIRE( t.value.rel_t == RelOpType::GT );
    }

    SECTION( "greater or equal" ) {
      iss.str(">=");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::REL_OP );
      REQUIRE( t.str == ">=" );
      REQUIRE( t.value.rel_t == RelOpType::GE );
    }

    SECTION( "filter" ) {
      iss.str(">?");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::PIPE_OP );
      REQUIRE( t.str == ">?" );
      REQUIRE( t.value.pipe_t == PipeOpType::FLT );
    }

    SECTION( "map" ) {
      iss.str(">:");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::PIPE_OP );
      REQUIRE( t.str == ">:" );
      REQUIRE( t.value.pipe_t == PipeOpType::MAP );
    }

    SECTION( "less than" ) {
      iss.str("<");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::REL_OP );
      REQUIRE( t.str == "<" );
      REQUIRE( t.value.rel_t == RelOpType::LT );
    }

    SECTION( "less or equal" ) {
      iss.str("<=");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::REL_OP );
      REQUIRE( t.str == "<=" );
      REQUIRE( t.value.rel_t == RelOpType::LE );
    }

    SECTION( "and" ) {
      iss.str("&&");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::AND );
      REQUIRE( t.str == "&&" );
    }

    SECTION( "or" ) {
      iss.str("||");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::OR );
      REQUIRE( t.str == "||" );
    }

    SECTION( "assign" ) {
      iss.str("=");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::ASN );
      REQUIRE( t.str == "=" );
    }

    SECTION( "equal" ) {
      iss.str("==");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::EQ_OP );
      REQUIRE( t.str == "==" );
      REQUIRE( t.value.eq_t == EqOpType::EQU );
    }

    SECTION( "not" ) {
      iss.str("!");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::NOT );
      REQUIRE( t.str == "!" );
    }

    SECTION( "not equal" ) {
      iss.str("!=");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::EQ_OP );
      REQUIRE( t.str == "!=" );
      REQUIRE( t.value.eq_t == EqOpType::NEQ );
    }

    SECTION( "add" ) {
      iss.str("+");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::ADD_OP );
      REQUIRE( t.str == "+" );
      REQUIRE( t.value.add_t == AddOpType::ADD );
    }

    SECTION( "subtract" ) {
      iss.str("-");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::ADD_OP );
      REQUIRE( t.str == "-" );
      REQUIRE( t.value.add_t == AddOpType::SUB );
    }

    SECTION( "multiply" ) {
      iss.str("*");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::MUL_OP );
      REQUIRE( t.str == "*" );
      REQUIRE( t.value.mul_t == MulOpType::MUL );
    }

    SECTION( "divide" ) {
      iss.str("/");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::MUL_OP );
      REQUIRE( t.str == "/" );
      REQUIRE( t.value.mul_t == MulOpType::DIV );
    }

    SECTION( "modulo" ) {
      iss.str("%");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::MUL_OP );
      REQUIRE( t.str == "%" );
      REQUIRE( t.value.mul_t == MulOpType::MOD );
    }
  } // operators

  SECTION( "other" ) {
    SECTION( "curly brackets" ) {
      iss.str("{}");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::CUR_OP );
      REQUIRE( t.str == "{" );

      t = lexer.getToken();
      REQUIRE( t.type == TokType::CUR_CL );
      REQUIRE( t.str == "}" );
    }

    SECTION( "round brackets" ) {
      iss.str("()");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::RND_OP );
      REQUIRE( t.str == "(" );

      t = lexer.getToken();
      REQUIRE( t.type == TokType::RND_CL );
      REQUIRE( t.str == ")" );
    }

    SECTION( "square brackets" ) {
      iss.str("[]");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::SQR_OP );
      REQUIRE( t.str == "[" );

      t = lexer.getToken();
      REQUIRE( t.type == TokType::SQR_CL );
      REQUIRE( t.str == "]" );
    }

    SECTION( "dot" ) {
      iss.str(".");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::DOT );
      REQUIRE( t.str == "." );
    }

    SECTION( "comma" ) {
      iss.str(",");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::CMM );
      REQUIRE( t.str == "," );
    }

    SECTION( "colon" ) {
      iss.str(":");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::COL );
      REQUIRE( t.str == ":" );
    }

    SECTION( "semicolon" ) {
      iss.str(";");

      t = lexer.getToken();
      REQUIRE( t.type == TokType::SCL );
      REQUIRE( t.str == ";" );
    }
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

TEST_CASE( "exception is thrown when integer too long", "[lexer]" ) {
  std::string s(LANGUST_MAX_INTEGER_LENGTH + 1, '1');
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
