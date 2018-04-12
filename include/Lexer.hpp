#ifndef LEXER_HPP
#define LEXER_HPP

#include "Input.hpp"
#include "Token.hpp"

#define MAX_IDENTIFIER_LENGTH 64

namespace langust {
namespace parse {

class Lexer {
public:
  Lexer(std::istream& in = std::cin);

  Token getToken();

private:
  Input input_;
  std::string buffer_;
  char lastch_;

  //int lineptr_;
  int line_;
  int col_;

  Token current_tok_;

private:
  void ignoreWhiteSpace();

  Token tryIdentifier();
  Token tryInteger();
  Token tryOperator();
  Token tryOther();

  Token tryBoolean();
  Token tryFunc();
  Token tryReturn();

  void processNextChar();
  void cacheNextChar();
};

} //parse
} //langust

#endif //LEXER_HPP
