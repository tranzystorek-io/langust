#include "Lexer.hpp"

#include <cctype>

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

  if( (ret = tryIdentifier()).isValid() )
    ;
  else if( (ret = tryInteger()).isValid() )
    ;
  else if( (ret = tryOperator()).isValid() )
    ;
  else if( (ret = tryOther()).isValid() )
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

Token Lexer::currentToken() const {
  return current_tok_;
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

      if(++length > LANGUST_MAX_IDENTIFIER_LENGTH) {
        throw std::runtime_error("Maximum identifier length exceeded");
      }
    }
  }
  else {
    //identifier check failed
    return ret;
  }

  if( (ret = tryKeyword()).isValid() ) {
    return ret;
  }

  //identifier
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
      int length = 1;
      cacheNextChar();

      while(std::isdigit(lastch_)) {
        cacheNextChar();

        if(++length > LANGUST_MAX_INTEGER_LENGTH) {
          throw std::runtime_error("Maximum integer length exceeded");
        }
      }

      ret.type = Type::INT;
      ret.str = buffer_;
      ret.value.i = std::stoi(buffer_);
    }
    else {
      //TODO error?
    }
  }
  else if(std::isdigit(lastch_)) {
    int length = 1;
    cacheNextChar();

    while(std::isdigit(lastch_)) {
      cacheNextChar();

      if(++length > LANGUST_MAX_INTEGER_LENGTH) {
        throw std::runtime_error("Maximum integer length exceeded");
      }
    }

    ret.type = Type::INT;
    ret.str = buffer_;
    ret.value.i = std::stoi(buffer_);
  }

  return ret;
}

Token Lexer::tryOperator() {
  Token ret = Token::Unknown();

  if( (ret = tryRelOpOrPipeOp()).isValid() )
    ;
  else if( (ret = tryMulOp()).isValid() )
    ;
  else if( (ret = tryAddOp()).isValid() )
    ;
  else if( (ret = tryEqOp()).isValid() )
    ;
  else if( (ret = tryLogicalOp()).isValid() )
    ;

  return ret;
}

Token Lexer::tryRelOpOrPipeOp() {
  Token ret = Token::Unknown();

  if(lastch_ == '<') {
    char test = input_.peekNextChar();

    if(test == '=') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::REL_OP;
      ret.str = buffer_;
      ret.value.rel_t = RelOpType::LE;
    }
    else {
      cacheNextChar();

      ret.type = Type::REL_OP;
      ret.str = buffer_;
      ret.value.rel_t = RelOpType::LT;
    }
  }
  else if(lastch_ == '>') {
    char test = input_.peekNextChar();

    if(test == '=') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::REL_OP;
      ret.str = buffer_;
      ret.value.rel_t = RelOpType::GE;
    }
    else if(test == '?') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::PIPE_OP;
      ret.str = buffer_;
      ret.value.pipe_t = PipeOpType::FLT;
    }
    else if(test == ':') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::PIPE_OP;
      ret.str = buffer_;
      ret.value.pipe_t = PipeOpType::MAP;
    }
    else {
      cacheNextChar();

      ret.type = Type::REL_OP;
      ret.str = buffer_;
      ret.value.rel_t = RelOpType::GT;
    }
  }

  return ret;
}

Token Lexer::tryMulOp() {
  Token ret = Token::Unknown();

  if(lastch_ == '*') {
    cacheNextChar();

    ret.type = Type::MUL_OP;
    ret.str = buffer_;
    ret.value.mul_t = MulOpType::MUL;
  }
  else if(lastch_ == '/') {
    cacheNextChar();

    ret.type = Type::MUL_OP;
    ret.str = buffer_;
    ret.value.mul_t = MulOpType::DIV;
  }
  else if(lastch_ == '%') {
    cacheNextChar();

    ret.type = Type::MUL_OP;
    ret.str = buffer_;
    ret.value.mul_t = MulOpType::MOD;
  }

  return ret;
}

Token Lexer::tryAddOp() {
  Token ret = Token::Unknown();

  if(lastch_ == '+') {
    cacheNextChar();

    ret.type = Type::ADD_OP;
    ret.str = buffer_;
    ret.value.add_t = AddOpType::ADD;
  }
  else if(lastch_ == '-') {
    cacheNextChar();

    ret.type = Type::ADD_OP;
    ret.str = buffer_;
    ret.value.add_t = AddOpType::SUB;
  }

  return ret;
}

Token Lexer::tryEqOp() {
  Token ret = Token::Unknown();

  if(lastch_ == '!') {
    char test = input_.peekNextChar();

    if(test == '=') {
      cacheNextChar();
      cacheNextChar();

      ret.type = Type::EQ_OP;
      ret.str = buffer_;
      ret.value.eq_t = EqOpType::NEQ;
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

      ret.type = Type::EQ_OP;
      ret.str = buffer_;
      ret.value.eq_t = EqOpType::EQU;
    }
    else {
      cacheNextChar();

      ret.type = Type::ASN;
      ret.str = buffer_;
    }
  }

  return ret;
}

Token Lexer::tryLogicalOp() {
  Token ret = Token::Unknown();

  if(lastch_ == '&') {
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

Token Lexer::tryKeyword() {
  Token ret = Token::Unknown();

  if( (ret = tryBoolean()).isValid() ) {
    return ret;
  }

  if( (ret = tryFunc()).isValid() ) {
    return ret;
  }

  if( (ret = tryReturn()).isValid() ) {
    return ret;
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
