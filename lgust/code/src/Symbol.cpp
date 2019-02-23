#include "Symbol.hpp"

using namespace langust::parse;

//name map
const char* Symbol::name_map[] = {
  "STMT",
  "STMT_LIST", "STMT_LIST_A",
  "RET",
  "PARAM_LIST", "PARAM_LIST_A",
  "F_LIT",
  "F_LIT_CONT", "F_LIT_CONT_A", "F_LIT_CONT_B",
  "LIST_LIT", "LIST_LIT_A",
  "LIST_IND", "LIST_IND_A", "LIST_IND_B",
  "ARG_LIST", "ARG_LIST_A",
  "ATOM",
  "ID_OR_FAPPLY",
  "OR_FAPPLY", "OR_FAPPLY_A",
  "LIST_EXPR", "LIST_EXPR_A", "LIST_EXPR_B",
  "MUL_EXPR", "MUL_EXPR_A",
  "ADD_EXPR", "ADD_EXPR_A",
  "REL_EXPR", "REL_EXPR_A",
  "EQ_EXPR", "EQ_EXPR_A",
  "NOT_EXPR",
  "AND_EXPR", "AND_EXPR_A",
  "OR_EXPR", "OR_EXPR_A",
  "EXPR", "EXPR_A"
};
