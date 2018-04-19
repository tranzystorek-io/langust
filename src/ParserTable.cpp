#include "ParserTable.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>

using namespace langust::parse;

langust::parse::ParserTable::ParserTable() {
  initTable();
}

Symbol ParserTable::getSymbol(SymbolId id, Token::Type t) const {
  try {
    const Row& row = table_.at(id);
    return row.at(t);
  }
  catch(const std::out_of_range& e) {
    return Symbol::Invalid();
  }
}

ParserTable::TokenList ParserTable::getTokensForSymbol(SymbolId id) const {
  TokenList ret;

  const Row& row = table_.at(id);

  std::transform(row.begin(),
                 row.end(),
                 std::back_inserter(ret),
                 [](const Row::value_type& v) {
                   return v.first;
                 });

  return ret;
}

void ParserTable::initTable() {
  using TokType = Token::Type;

  Symbol s;
  Row r;

  //STMT
  s.setRule(SymbolId::STMT, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::STMT, std::move(r));
  r.clear();

  //STMT_LIST
  s.setRule(SymbolId::STMT_LIST, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::STMT_LIST, std::move(r));
  r.clear();

  //STMT_LIST_A
  s.setRule(SymbolId::STMT_LIST_A, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::RET, s);
  r.emplace(TokType::CUR_CL, s);

  table_.emplace(SymbolId::STMT_LIST_A, std::move(r));
  r.clear();

  //RET
  s.setRule(SymbolId::RET, 0);

  r.emplace(TokType::RET, s);

  table_.emplace(SymbolId::RET, std::move(r));
  r.clear();

  //PARAM_LIST
  s.setRule(SymbolId::PARAM_LIST, 0);

  r.emplace(TokType::IDN, s);

  table_.emplace(SymbolId::PARAM_LIST, std::move(r));
  r.clear();

  //PARAM_LIST_A
  s.setRule(SymbolId::PARAM_LIST_A, 0);

  r.emplace(TokType::CMM, s);

  s.rule.index = 1;

  r.emplace(TokType::RND_CL, s);

  table_.emplace(SymbolId::PARAM_LIST_A, std::move(r));
  r.clear();

  //F_LIT
  s.setRule(SymbolId::F_LIT, 0);

  r.emplace(TokType::FUN, s);

  table_.emplace(SymbolId::F_LIT, std::move(r));
  r.clear();

  //F_LIT_CONT
  s.setRule(SymbolId::F_LIT_CONT, 0);

  r.emplace(TokType::IDN, s);

  s.rule.index = 1;

  r.emplace(TokType::RND_CL, s);

  table_.emplace(SymbolId::F_LIT_CONT, std::move(r));
  r.clear();

  //F_LIT_CONT_A
  s.setRule(F_LIT_CONT_A, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::RET, s);
  r.emplace(TokType::CUR_CL, s);

  table_.emplace(SymbolId::F_LIT_CONT_A, std::move(r));
  r.clear();

  //F_LIT_CONT_B
  s.setRule(SymbolId::F_LIT_CONT_B, 0);

  r.emplace(TokType::RET, s);

  s.rule.index = 1;

  r.emplace(TokType::CUR_CL, s);

  table_.emplace(SymbolId::F_LIT_CONT_B, std::move(r));
  r.clear();

  //LIST_LIT
  s.setRule(SymbolId::LIST_LIT, 0);

  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::LIST_LIT, std::move(r));
  r.clear();

  //LIST_LIT_A
  s.setRule(SymbolId::LIST_LIT_A, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::LIST_LIT_A, std::move(r));
  r.clear();

  //LIST_IND
  s.setRule(SymbolId::LIST_IND, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::COL, s);

  table_.emplace(SymbolId::LIST_IND, std::move(r));
  r.clear();

  //LIST_IND_A
  s.setRule(SymbolId::LIST_IND_A, 0);

  r.emplace(TokType::COL, s);

  s.rule.index = 1;

  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::LIST_IND_A, std::move(r));
  r.clear();

  //LIST_IND_B
  s.setRule(SymbolId::LIST_IND_B, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::LIST_IND_B, std::move(r));
  r.clear();

  //ARG_LIST
  s.setRule(SymbolId::ARG_LIST, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::ARG_LIST, std::move(r));
  r.clear();

  //ARG_LIST_A
  s.setRule(SymbolId::ARG_LIST_A, 0);

  r.emplace(TokType::CMM, s);

  s.rule.index = 1;

  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::ARG_LIST_A, std::move(r));
  r.clear();

  //ATOM
  s.setRule(SymbolId::ATOM, 0);
  r.emplace(TokType::INT, s);

  s.rule.index = 1;
  r.emplace(TokType::BLN, s);

  s.rule.index = 2;
  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 3;
  r.emplace(TokType::FUN, s);

  s.rule.index = 4;
  r.emplace(TokType::IDN, s);

  s.rule.index = 5;
  r.emplace(TokType::RND_OP, s);

  table_.emplace(SymbolId::ATOM, std::move(r));
  r.clear();

  //ID_OR_FAPPLY
  s.setRule(SymbolId::ID_OR_FAPPLY, 0);

  r.emplace(TokType::IDN, s);

  table_.emplace(SymbolId::ID_OR_FAPPLY, std::move(r));
  r.clear();

  //OR_FAPPLY
  s.setRule(SymbolId::OR_FAPPLY, 0);

  r.emplace(TokType::DOT, s);

  s.rule.index = 1;

  r.emplace(TokType::PIPE_OP, s);
  r.emplace(TokType::MUL_OP, s);
  r.emplace(TokType::ADD_OP, s);
  r.emplace(TokType::REL_OP, s);
  r.emplace(TokType::EQ_OP, s);
  r.emplace(TokType::AND, s);
  r.emplace(TokType::OR, s);
  r.emplace(TokType::ASN, s);
  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::OR_FAPPLY, std::move(r));
  r.clear();

  //OR_FAPPLY_A
  s.setRule(SymbolId::OR_FAPPLY_A, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::RND_CL, s);

  table_.emplace(SymbolId::OR_FAPPLY_A, std::move(r));
  r.clear();

  //LIST_EXPR
  s.setRule(SymbolId::LIST_EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::LIST_EXPR, std::move(r));
  r.clear();

  //LIST_EXPR_A
  s.setRule(SymbolId::LIST_EXPR_A, 0);

  r.emplace(TokType::PIPE_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 2;

  r.emplace(TokType::MUL_OP, s);
  r.emplace(TokType::ADD_OP, s);
  r.emplace(TokType::REL_OP, s);
  r.emplace(TokType::EQ_OP, s);
  r.emplace(TokType::AND, s);
  r.emplace(TokType::OR, s);
  r.emplace(TokType::ASN, s);
  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::LIST_EXPR_A, std::move(r));
  r.clear();

  //LIST_EXPR_B
  s.setRule(SymbolId::LIST_EXPR_B, 0);

  r.emplace(TokType::IDN, s);

  s.rule.index = 1;

  r.emplace(TokType::FUN, s);

  table_.emplace(SymbolId::LIST_EXPR_B, std::move(r));
  r.clear();

  //MUL_EXPR
  s.setRule(SymbolId::MUL_EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::MUL_EXPR, std::move(r));
  r.clear();

  //MUL_EXPR_A
  s.setRule(SymbolId::MUL_EXPR_A, 0);

  r.emplace(TokType::MUL_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::ADD_OP, s);
  r.emplace(TokType::REL_OP, s);
  r.emplace(TokType::EQ_OP, s);
  r.emplace(TokType::AND, s);
  r.emplace(TokType::OR, s);
  r.emplace(TokType::ASN, s);
  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::MUL_EXPR_A, std::move(r));
  r.clear();

  //ADD_EXPR
  s.setRule(SymbolId::ADD_EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::ADD_EXPR, std::move(r));
  r.clear();

  //ADD_EXPR_A
  s.setRule(SymbolId::ADD_EXPR_A, 0);

  r.emplace(TokType::ADD_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::REL_OP, s);
  r.emplace(TokType::EQ_OP, s);
  r.emplace(TokType::AND, s);
  r.emplace(TokType::OR, s);
  r.emplace(TokType::ASN, s);
  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::ADD_EXPR_A, std::move(r));
  r.clear();

  //REL_EXPR
  s.setRule(SymbolId::REL_EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::REL_EXPR, std::move(r));
  r.clear();

  //REL_EXPR_A
  s.setRule(SymbolId::REL_EXPR_A, 0);

  r.emplace(TokType::REL_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::EQ_OP, s);
  r.emplace(TokType::AND, s);
  r.emplace(TokType::OR, s);
  r.emplace(TokType::ASN, s);
  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::REL_EXPR_A, std::move(r));
  r.clear();

  //EQ_EXPR
  s.setRule(SymbolId::EQ_EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::EQ_EXPR, std::move(r));
  r.clear();

  //EQ_EXPR_A
  s.setRule(SymbolId::EQ_EXPR_A, 0);

  r.emplace(TokType::EQ_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::AND, s);
  r.emplace(TokType::OR, s);
  r.emplace(TokType::ASN, s);
  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::EQ_EXPR_A, std::move(r));
  r.clear();

  //NOT_EXPR
  s.setRule(SymbolId::NOT_EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  s.rule.index = 1;

  r.emplace(TokType::NOT, s);

  table_.emplace(SymbolId::NOT_EXPR, std::move(r));
  r.clear();

  //AND_EXPR
  s.setRule(SymbolId::AND_EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::AND_EXPR, std::move(r));
  r.clear();

  //AND_EXPR_A
  s.setRule(SymbolId::AND_EXPR_A, 0);

  r.emplace(TokType::AND, s);

  s.rule.index = 1;

  r.emplace(TokType::OR, s);
  r.emplace(TokType::ASN, s);
  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::AND_EXPR_A, std::move(r));
  r.clear();

  //OR_EXPR
  s.setRule(SymbolId::OR_EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::OR_EXPR, std::move(r));
  r.clear();

  //OR_EXPR_A
  s.setRule(SymbolId::OR_EXPR_A, 0);

  r.emplace(TokType::OR, s);

  s.rule.index = 1;

  r.emplace(TokType::ASN, s);
  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::OR_EXPR_A, std::move(r));
  r.clear();

  //EXPR
  s.setRule(SymbolId::EXPR, 0);

  r.emplace(TokType::FUN, s);
  r.emplace(TokType::BLN, s);
  r.emplace(TokType::INT, s);
  r.emplace(TokType::IDN, s);
  r.emplace(TokType::NOT, s);
  r.emplace(TokType::RND_OP, s);
  r.emplace(TokType::SQR_OP, s);

  table_.emplace(SymbolId::EXPR, std::move(r));
  r.clear();

  //EXPR_A
  s.setRule(SymbolId::EXPR_A, 0);

  r.emplace(TokType::ASN, s);

  s.rule.index = 1;

  r.emplace(TokType::COL, s);
  r.emplace(TokType::SCL, s);
  r.emplace(TokType::CMM, s);
  r.emplace(TokType::RND_CL, s);
  r.emplace(TokType::SQR_CL, s);

  table_.emplace(SymbolId::EXPR_A, std::move(r));
} //initTable
