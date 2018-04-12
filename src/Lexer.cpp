#include "Lexer.hpp"

#include <cctype>
#include <cstdlib>

#include <stdexcept>

using namespace langust::parse;

using Type = Token::Type;

Lexer::Lexer(std::istream& in)
  : input_(in),
    lastch_(' '),
    //lineptr_(0),
    line_(1),
    col_(-1),
    current_tok_(Token::Unknown()) {
}

void Lexer::processNextChar() {
  if(lastch_ == '\n') {
    //lineptr_ = input_.getAbsPos();
    col_ = 0;
    ++line_;
  }

  else
    ++col_;

  lastch_ = input_.getNextChar();
}

void Lexer::cacheNextChar() {
  buffer_ += lastch_;
  //lastch_ = input_.getNextChar();
  processNextChar();
}

Token Lexer::getToken() {
  Token ret = Token::Unknown();

  ignoreWhiteSpace();

  if( (ret = tryIdentifier()) ){
    buffer_.clear();
    return current_tok_ = ret;
  }

  if( (ret = tryInteger()) ) {
    buffer_.clear();
    return current_tok_ = ret;
  }

  if( (ret = tryOperator()) ) {
    buffer_.clear();
    return current_tok_ = ret;
  }

  if( (ret = tryOther()) ) {
    buffer_.clear();
    return current_tok_ = ret;
  }

  //invalid symbol
  ret.setPos(line_, col_);
  cacheNextChar();
  ret.str = buffer_;

  buffer_.clear();
  return current_tok_ = ret;
}

void Lexer::ignoreWhiteSpace() {
  while(isspace(lastch_))
    processNextChar();
}

Token Lexer::tryIdentifier() {
  Token ret = Token::Unknown();

  int ln = line_;
  int cl = col_;

  if(isalpha(lastch_) || lastch_ == '_') {
    int length = 1;
    cacheNextChar();

    while(isalnum(lastch_) || lastch_ == '_') {
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
    ret.setPos(ln, cl);
    return ret;
  }

  if( (ret = tryFunc()) ) {
    ret.setPos(ln, cl);
    return ret;
  }

  if( (ret = tryReturn()) ) {
    ret.setPos(ln, cl);
    return ret;
  }

  ret.setPos(ln, cl);
  ret.type = Type::IDN;
  ret.str = buffer_;

  return ret;
}

Token Lexer::tryInteger() {
  Token ret = Token::Unknown();

  if(lastch_ == '0') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::INT;
    ret.str = buffer_;
    ret.value.i = 0;
  }
  else if(lastch_ == '-') {
    char test = input_.peekNextChar();

    if(test != '0' && isdigit(test)) {
      ret.setPos(line_, col_);
      cacheNextChar();

      while(isdigit(lastch_)) {
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
  else if(isdigit(lastch_)) {
    ret.setPos(line_, col_);
    cacheNextChar();

    while(isdigit(lastch_)) {
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
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::ADD;
    ret.str = buffer_;
  }
  else if(lastch_ == '-') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::SUB;
    ret.str = buffer_;
  }
  else if(lastch_ == '*') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::MUL;
    ret.str = buffer_;
  }
  else if(lastch_ == '/') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::DIV;
    ret.str = buffer_;
  }
  else if(lastch_ == '%') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::MOD;
    ret.str = buffer_;
  }
  else if(lastch_ == '!') {
    char test = input_.peekNextChar();

    if(test == '=') {
      ret.setPos(line_, col_);
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::NEQ;
      ret.str = buffer_;
    }
    else {
      ret.setPos(line_, col_);
      cacheNextChar();

      ret.type = Type::NOT;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '=') {
    char test = input_.peekNextChar();

    if(test == '=') {
      ret.setPos(line_, col_);
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::EQU;
      ret.str = buffer_;
    }
    else {
      ret.setPos(line_, col_);
      cacheNextChar();

      ret.type = Type::ASN;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '<') {
    char test = input_.peekNextChar();

    if(test == '=') {
      ret.setPos(line_, col_);
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::LE;
      ret.str = buffer_;
    }
    else {
      ret.setPos(line_, col_);
      cacheNextChar();

      ret.type = Type::LT;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '>') {
    char test = input_.peekNextChar();

    if(test == '=') {
      ret.setPos(line_, col_);
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::GE;
      ret.str = buffer_;
    }
    else if(test == '?') {
      ret.setPos(line_, col_);
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::FLT;
      ret.str = buffer_;
    }
    else if(test == ':') {
      ret.setPos(line_, col_);
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::MAP;
      ret.str = buffer_;
    }
    else {
      ret.setPos(line_, col_);
      cacheNextChar();

      ret.type = Type::GT;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '&') {
    char test = input_.peekNextChar();

    if(test == '&') {
      ret.setPos(line_, col_);
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::AND;
      ret.str = buffer_;
    }
  }
  else if(lastch_ == '|') {
    char test = input_.peekNextChar();

    if(test == '|') {
      ret.setPos(line_, col_);
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
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::CUR_OP;
    ret.str = buffer_;
  }
  else if(lastch_ == '}') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::CUR_CL;
    ret.str = buffer_;
  }
  else if(lastch_ == '(') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::RND_OP;
    ret.str = buffer_;
  }
  else if(lastch_ == ')') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::RND_CL;
    ret.str = buffer_;
  }
  else if(lastch_ == '[') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::SQR_OP;
    ret.str = buffer_;
  }
  else if(lastch_ == ']') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::SQR_CL;
    ret.str = buffer_;
  }
  else if(lastch_ == '.') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::DOT;
    ret.str = buffer_;
  }
  else if(lastch_ == ',') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::CMM;
    ret.str = buffer_;
  }
  else if(lastch_ == ':') {
    ret.setPos(line_, col_);
    cacheNextChar();

    ret.type = Type::COL;
    ret.str = buffer_;
  }
  else if(lastch_ == ';') {
    ret.setPos(line_, col_);
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
