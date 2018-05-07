#include "Token.hpp"

using namespace langust::parse;

//name map
const char* Token::name_map[] = {
  "INT", "IDN", "BLN",
  "FUN", "RET",
  "CUR_OP", "CUR_CL",
  "RND_OP", "RND_CL",
  "SQR_OP", "SQR_CL",
  "SCL", "COL", "DOT", "CMM",
  "PIPE_OP",
  "MUL_OP", "ADD_OP",
  "REL_OP", "EQ_OP",
  "NOT", "AND", "OR",
  "ASN",
  "END_OF_SEQ",
  "UNKNOWN"
};
