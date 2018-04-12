#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace langust {
namespace parse {

struct Token {
  enum class Type {
    INT, IDN, BLN,
    FUN, RET,
    CUR_OP, CUR_CL,
    RND_OP, RND_CL,
    SQR_OP, SQR_CL,
    SCL, COL, DOT, CMM,
    NOT, AND, OR,
    EQU, NEQ,
    FLT, MAP,
    ASN,
    MUL, DIV, MOD,
    ADD, SUB,
    LT, LE,
    GT, GE,
    END_OF_SEQ,
    UNKNOWN
  } type;

  std::string str;

  struct {
    int line;
    int col;
  } pos;

  union {
    int i;
    bool b;
  } value;

  Token(Type type, const std::string& s = "")
    : type(type), str(s)
  {}

  void setPos(int line, int col) {
    pos.line = line;
    pos.col = col;
  }

  operator bool() {
    return type != Type::UNKNOWN;
  }

  static Token Unknown() {
    return Token(Type::UNKNOWN);
  }
};

} //parse
} //langust

#endif //TOKEN_HPP
