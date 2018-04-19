#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "Token.hpp"

namespace langust {
namespace parse {

enum SymbolId {
  STMT,
  STMT_LIST, STMT_LIST_A,
  RET,
  PARAM_LIST, PARAM_LIST_A,
  F_LIT,
  F_LIT_CONT, F_LIT_CONT_A, F_LIT_CONT_B,
  LIST_LIT, LIST_LIT_A,
  LIST_IND, LIST_IND_A, LIST_IND_B,
  ARG_LIST, ARG_LIST_A,
  ATOM,
  ID_OR_FAPPLY,
  OR_FAPPLY, OR_FAPPLY_A,
  LIST_EXPR, LIST_EXPR_A, LIST_EXPR_B,
  MUL_EXPR, MUL_EXPR_A,
  ADD_EXPR, ADD_EXPR_A,
  REL_EXPR, REL_EXPR_A,
  EQ_EXPR, EQ_EXPR_A,
  NOT_EXPR,
  AND_EXPR, AND_EXPR_A,
  OR_EXPR, OR_EXPR_A,
  EXPR, EXPR_A
};

struct RuleAssoc {
  SymbolId id;
  int index;

  void setValue(SymbolId id, int index) {
    this->id = id;
    this->index = index;
  }
};

struct Symbol {
  enum Type {
    OK,
    INVALID
  } type;

  RuleAssoc rule;

  Symbol()
    : type(Type::INVALID) {
  }

  Symbol(const RuleAssoc& ra)
    : rule(ra) {
  }

  void setRule(SymbolId id, int index) {
    rule.setValue(id, index);
  }

  static Symbol Invalid() {
    return Symbol();
  }

  bool isValid() const {
    return type != Type::INVALID;
  }
};

} //parse
} //langust

#endif //SYMBOL_HPP
