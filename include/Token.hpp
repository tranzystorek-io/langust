#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

namespace langust {
namespace parse {

struct Position {
  Position(int l = 0, int c = 0)
    : line(l),
      col(c) {
  }

  int line;
  int col;
};

enum PipeOpType {
  FLT,
  MAP
};

enum MulOpType {
  MUL,
  DIV,
  MOD
};

enum AddOpType {
  ADD,
  SUB
};

enum RelOpType {
  LT, LE,
  GT, GE
};

enum EqOpType {
  EQU,
  NEQ
};

struct Token {
  enum class Type {
    INT, IDN, BLN,
    FUN, RET,
    CUR_OP, CUR_CL,
    RND_OP, RND_CL,
    SQR_OP, SQR_CL,
    SCL, COL, DOT, CMM,
    PIPE_OP,
    MUL_OP,
    ADD_OP,
    REL_OP,
    EQ_OP,
    NOT, AND, OR,
    ASN,
    END_OF_SEQ,
    UNKNOWN
  } type;

  std::string str;

  Position pos;

  union {
    int i;
    bool b;

    PipeOpType pipe_t;
    MulOpType mul_t;
    AddOpType add_t;
    RelOpType rel_t;
    EqOpType eq_t;
  } value;

  Token(Type type, const std::string& s = "")
    : type(type),
      str(s) {
  }

  void setPos(const Position& p) {
    pos = p;
  }

  void setPos(int line, int col) {
    pos.line = line;
    pos.col = col;
  }

  bool isValid() const {
    return type != Type::UNKNOWN;
  }

  operator bool() const {
    return type != Type::UNKNOWN;
  }

  static Token Unknown() {
    return Token(Type::UNKNOWN);
  }
};

} //parse
} //langust

#endif //TOKEN_HPP
