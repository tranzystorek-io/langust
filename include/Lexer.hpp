#ifndef LEXER_HPP
#define LEXER_HPP

#include <functional>

#include "Input.hpp"
#include "Token.hpp"

#ifndef LANGUST_MAX_IDENTIFIER_LENGTH
#define LANGUST_MAX_IDENTIFIER_LENGTH 64
#endif // LANGUST_MAX_IDENTIFIER_LENGTH

#ifndef LANGUST_MAX_INTEGER_LENGTH
#define LANGUST_MAX_INTEGER_LENGTH 10
#endif // LANGUST_MAX_INTEGER_LENGTH

namespace langust {
namespace parse {

class Lexer {
public:
  using Callback = std::function<void()>;

public:
  // Lexer(std::istream& in = std::cin, const Callback& callback = Callback());
  Lexer(Input& input);

  Token getToken();
  Token currentToken() const;

  void ignoreLine();

private:
  Input& input_;
  Callback callback_;

  char lastch_;
  std::string buffer_;

  Position pos_;
  Token current_tok_;

private:
  void ignoreWhiteSpace();

  Token tryIdentifier();
  Token tryInteger();

  Token tryOperator();
  Token tryRelOpOrPipeOp();
  Token tryMulOp();
  Token tryAddOp();
  Token tryEqOp();
  Token tryLogicalOp();

  Token tryOther();

  Token tryKeyword();
  Token tryBoolean();
  Token tryFunc();
  Token tryReturn();

  void processNextChar();
  void cacheNextChar();
};

} //parse
} //langust

#endif //LEXER_HPP
