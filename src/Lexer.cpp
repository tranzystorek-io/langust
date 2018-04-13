#include "Lexer.hpp"

#include <cctype>
#include <cstdlib>

#include <stdexcept>

using namespace langust::parse;

using Type = Token::Type;

Lexer::Lexer(std::istream& in)
  : input_(in),
    lastch_(' '),
    pos_(1, -1),
    current_tok_(Token::Unknown()) {
}

void Lexer::processNextChar() {
  if(lastch_ == '\n') {
    pos_.col = 0;
    ++pos_.line;
  }

  else
    ++pos_.col;

  lastch_ = input_.getNextChar();
}

void Lexer::cacheNextChar() {
  buffer_ += lastch_;
  processNextChar();
}

Token Lexer::getToken() {
  Token ret = Token::Unknown();

  ignoreWhiteSpace();
  Position pos = pos_;

  if( (ret = tryIdentifier()) )
    ;
  else if( (ret = tryInteger()) )
    ;
  else if( (ret = tryOperator()) )
    ;
  else if( (ret = tryOther()) )
    ;
  else {
    //invalid symbol
    cacheNextChar();
    ret.str = buffer_;
  }

  ret.setPos(pos);
  buffer_.clear();
  return current_tok_ = ret;
}

void Lexer::ignoreWhiteSpace() {
  while(std::isspace(lastch_))
    processNextChar();
}

Token Lexer::tryIdentifier() {
  Token ret = Token::Unknown();

  if(std::isalpha(lastch_) || lastch_ == '_') {
    int length = 1;
    cacheNextChar();

    while(std::isalnum(lastch_) || lastch_ == '_') {
      cacheNextChar();

      if(++length > MAX_IDENTIFIER_LENGTH) {
        throw std::runtime_error("Maximum identifier length exceeded");
      }
    }
  }
  else {
    //identifier check failed
    return ret;
  }

  if( (ret = tryBoolean()) ) {
    return ret;
  }

  if( (ret = tryFunc()) ) {
    return ret;
  }

  if( (ret = tryReturn()) ) {
    return ret;
  }

  ret.type = Type::IDN;
  ret.str = buffer_;

  return ret;
}

Token Lexer::tryInteger() {
  Token ret = Token::Unknown();

  if(lastch_ == '0') {
    cacheNextChar();

    ret.type = Type::INT;
    ret.str = buffer_;
    ret.value.i = 0;
  }
  else if(lastch_ == '-') {
    char test = input_.peekNextChar();

    if(test != '0' && std::isdigit(test)) {
      cacheNextChar();

      while(std::isdigit(lastch_)) {
        cacheNextChar();
      }

      ret.type = Type::INT;
      ret.str = buffer_;
      ret.value.i = atoi(buffer_.c_str());
    }
    else {
      //TODO error?
    }
  }
  else if(std::isdigit(lastch_)) {
    cacheNextChar();

    while(std::isdigit(lastch_)) {
      cacheNextChar();
    }

    ret.type = Type::INT;
    ret.str = buffer_;
    ret.value.i = atoi(buffer_.c_str());
  }

  return ret;
}

Token Lexer::tryOperator() {
  Token ret = Token::Unknown();

  if(lastch_ == '+') {
    cacheNextChar();

    ret.type = Type::ADD;
    ret.str = buffer_;
  }
  else if(lastch_ == '-') {
    cacheNextChar();

    ret.type = Type::SUB;
    ret.str = buffer_;
  }
  else if(lastch_ == '*') {
    cacheNextChar();

    ret.type = Type::MUL;
    ret.str = buffer_;
  }
  else if(lastch_ == '/') {
    cacheNextChar();

    ret.type = Type::DIV;
    ret.str = buffer_;
  }
  else if(lastch_ == '%') {
    cacheNextChar();

    ret.type = Type::MOD;
    ret.str = buffer_;
  }
  else if(lastch_ == '!') {
    char test = input_.peekNextChar();

    if(test == '=') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::NEQ;
      ret.str = buffer_;
    }
    else {
      cacheNextChar();

      ret.type = Type::NOT;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '=') {
    char test = input_.peekNextChar();

    if(test == '=') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::EQU;
      ret.str = buffer_;
    }
    else {
      cacheNextChar();

      ret.type = Type::ASN;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '<') {
    char test = input_.peekNextChar();

    if(test == '=') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::LE;
      ret.str = buffer_;
    }
    else {
      cacheNextChar();

      ret.type = Type::LT;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '>') {
    char test = input_.peekNextChar();

    if(test == '=') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::GE;
      ret.str = buffer_;
    }
    else if(test == '?') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::FLT;
      ret.str = buffer_;
    }
    else if(test == ':') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::MAP;
      ret.str = buffer_;
    }
    else {
      cacheNextChar();

      ret.type = Type::GT;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '&') {
    char test = input_.peekNextChar();

    if(test == '&') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::AND;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '|') {
    char test = input_.peekNextChar();

    if(test == '|') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::OR;
      ret.str = buffer_;
    }
  }

  return ret;
}

Token Lexer::tryOther() {
  Token ret = Token::Unknown();

  if(lastch_ == '{') {
    cacheNextChar();

    ret.type = Type::CUR_OP;
    ret.str = buffer_;
  }
  else if(lastch_ == '}') {
    cacheNextChar();

    ret.type = Type::CUR_CL;
    ret.str = buffer_;
  }
  else if(lastch_ == '(') {
    cacheNextChar();

    ret.type = Type::RND_OP;
    ret.str = buffer_;
  }
  else if(lastch_ == ')') {
    cacheNextChar();

    ret.type = Type::RND_CL;
    ret.str = buffer_;
  }
  else if(lastch_ == '[') {
    cacheNextChar();

    ret.type = Type::SQR_OP;
    ret.str = buffer_;
  }
  else if(lastch_ == ']') {
    cacheNextChar();

    ret.type = Type::SQR_CL;
    ret.str = buffer_;
  }
  else if(lastch_ == '.') {
    cacheNextChar();

    ret.type = Type::DOT;
    ret.str = buffer_;
  }
  else if(lastch_ == ',') {
    cacheNextChar();

    ret.type = Type::CMM;
    ret.str = buffer_;
  }
  else if(lastch_ == ':') {
    cacheNextChar();

    ret.type = Type::COL;
    ret.str = buffer_;
  }
  else if(lastch_ == ';') {
    cacheNextChar();

    ret.type = Type::SCL;
    ret.str = buffer_;
  }

  return ret;
}

Token Lexer::tryBoolean() {
  Token ret = Token::Unknown();

  if(buffer_ == "False") {
    ret.type = Type::BLN;
    ret.str = buffer_;
    ret.value.b = false;
  }
  else if(buffer_ == "True") {
    ret.type = Type::BLN;
    ret.str = buffer_;
    ret.value.b = true;
  }

  return ret;
}

Token Lexer::tryFunc() {
  Token ret = Token::Unknown();

  if(buffer_ == "func") {
    ret.type = Type::FUN;
    ret.str = buffer_;
  }

  return ret;
}

Token Lexer::tryReturn() {
  Token ret = Token::Unknown();

  if(buffer_ == "return") {
    ret.type = Type::RET;
    ret.str = buffer_;
  }

  return ret;
}
