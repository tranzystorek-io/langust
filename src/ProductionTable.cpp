#include "ProductionTable.hpp"

using namespace langust::parse;

ProductionTable::ProductionTable() {
  initTable();
}

const ProductionTable::Production& ProductionTable::getProduction(SymbolId id, int index) const {
  return table_.at(id).at(index);
}

void ProductionTable::initTable() {
  using PS = ProdSymbol;
  using S = SymbolId;
  using T = Token::Type;

  ProdList pl;

  //STMT
  pl.push_back({PS::NonTerminal(S::EXPR),
        PS::Terminal(T::SCL)} );

  table_.emplace(S::STMT, std::move(pl));
  pl.clear();

  //STMT_LIST
  pl.push_back({PS::NonTerminal(S::STMT),
        PS::NonTerminal(S::STMT_LIST_A)});

  table_.emplace(S::STMT_LIST, std::move(pl));
  pl.clear();

  //STMT_LIST_A
  pl.push_back({PS::NonTerminal(S::STMT),
        PS::NonTerminal(S::STMT_LIST_A)});

  pl.push_back({});

  table_.emplace(S::STMT_LIST_A, std::move(pl));
  pl.clear();

  //RET
  pl.push_back({PS::Terminal(T::RET),
        PS::NonTerminal(S::EXPR),
        PS::Terminal(T::SCL)});

  table_.emplace(S::RET, std::move(pl));
  pl.clear();

  //PARAM_LIST
  pl.push_back({PS::Terminal(T::IDN),
        PS::NonTerminal(S::PARAM_LIST_A)});

  table_.emplace(S::PARAM_LIST, std::move(pl));
  pl.clear();

  //PARAM_LIST_A
  pl.push_back({PS::Terminal(T::CMM),
        PS::Terminal(T::IDN),
        PS::NonTerminal(S::PARAM_LIST_A)});

  pl.push_back({});

  table_.emplace(S::PARAM_LIST_A, std::move(pl));
  pl.clear();

  //F_LIT
  pl.push_back({PS::Terminal(T::FUN),
        PS::Terminal(T::RND_OP),
        PS::NonTerminal(S::F_LIT_CONT)});

  table_.emplace(S::F_LIT, std::move(pl));
  pl.clear();

  //F_LIT_CONT
  pl.push_back({PS::NonTerminal(S::PARAM_LIST),
        PS::Terminal(T::RND_CL),
        PS::Terminal(T::CUR_OP),
        PS::NonTerminal(S::F_LIT_CONT_A)});

  pl.push_back({PS::Terminal(T::RND_CL),
        PS::Terminal(T::CUR_OP),
        PS::NonTerminal(S::F_LIT_CONT_A)});

  table_.emplace(S::F_LIT_CONT, std::move(pl));
  pl.clear();

  //F_LIT_CONT_A
  pl.push_back({PS::NonTerminal(S::STMT_LIST),
        PS::NonTerminal(S::F_LIT_CONT_B)});

  pl.push_back({PS::NonTerminal(S::F_LIT_CONT_B)});

  table_.emplace(S::F_LIT_CONT_A, std::move(pl));
  pl.clear();

  //F_LIT_CONT_B
  pl.push_back({PS::NonTerminal(S::RET),
        PS::Terminal(T::CUR_CL)});

  pl.push_back({PS::Terminal(T::CUR_CL)});

  table_.emplace(S::F_LIT_CONT_B, std::move(pl));
  pl.clear();

  //LIST_LIT
  pl.push_back({PS::Terminal(T::SQR_OP),
        PS::NonTerminal(S::LIST_LIT_A)});

  table_.emplace(S::LIST_LIT, std::move(pl));
  pl.clear();

  //LIST_LIT_A
  pl.push_back({PS::NonTerminal(S::ARG_LIST),
        PS::Terminal(T::SQR_CL)});

  pl.push_back({PS::Terminal(T::SQR_CL)});

  table_.emplace(S::LIST_LIT_A, std::move(pl));
  pl.clear();

  //LIST_IND
  pl.push_back({PS::NonTerminal(S::EXPR),
        PS::NonTerminal(S::LIST_IND_A)});

  pl.push_back({PS::Terminal(T::COL),
        PS::NonTerminal(S::EXPR)});

  table_.emplace(S::LIST_IND, std::move(pl));
  pl.clear();

  //LIST_IND_A
  pl.push_back({PS::Terminal(T::COL),
        PS::NonTerminal(S::LIST_IND_B)});

  pl.push_back({});

  table_.emplace(S::LIST_IND_A, std::move(pl));
  pl.clear();

  //LIST_IND_B
  pl.push_back({PS::NonTerminal(S::EXPR)});

  pl.push_back({});

  table_.emplace(S::LIST_IND_B, std::move(pl));
  pl.clear();

  //ARG_LIST
  pl.push_back({PS::NonTerminal(S::EXPR),
        PS::NonTerminal(S::ARG_LIST_A)});

  table_.emplace(S::ARG_LIST, std::move(pl));
  pl.clear();

  //ARG_LIST_A
  pl.push_back({PS::Terminal(T::CMM),
        PS::NonTerminal(S::EXPR),
        PS::NonTerminal(S::ARG_LIST_A)});

  pl.push_back({});

  table_.emplace(S::ARG_LIST_A, std::move(pl));
  pl.clear();

  //ATOM
  pl.push_back({PS::Terminal(T::INT)});
  pl.push_back({PS::Terminal(T::BLN)});
  pl.push_back({PS::NonTerminal(S::LIST_LIT)});
  pl.push_back({PS::NonTerminal(S::F_LIT)});
  pl.push_back({PS::NonTerminal(S::ID_OR_FAPPLY)});
  pl.push_back({PS::Terminal(T::RND_OP),
        PS::NonTerminal(S::EXPR),
        PS::Terminal(T::RND_CL)});

  table_.emplace(S::ATOM, std::move(pl));
  pl.clear();

  //ID_OR_FAPPLY
  pl.push_back({PS::Terminal(T::IDN),
        PS::NonTerminal(S::OR_FAPPLY)});

  table_.emplace(S::ID_OR_FAPPLY, std::move(pl));
  pl.clear();

  //OR_FAPPLY
  pl.push_back({PS::Terminal(T::DOT),
        PS::Terminal(T::RND_OP),
        PS::NonTerminal(S::OR_FAPPLY_A)});

  pl.push_back({});

  table_.emplace(S::OR_FAPPLY, std::move(pl));
  pl.clear();

  //OR_FAPPLY_A
  pl.push_back({PS::NonTerminal(S::ARG_LIST),
        PS::Terminal(T::RND_CL)});

  pl.push_back({PS::Terminal(T::RND_CL)});

  table_.emplace(S::OR_FAPPLY_A, std::move(pl));
  pl.clear();

  //LIST_EXPR
  pl.push_back({PS::NonTerminal(S::ATOM),
        PS::NonTerminal(S::LIST_EXPR_A)});

  table_.emplace(S::LIST_EXPR, std::move(pl));
  pl.clear();

  //LIST_EXPR_A
  pl.push_back({PS::Terminal(T::PIPE_OP),
        PS::NonTerminal(S::LIST_EXPR_B),
        PS::NonTerminal(S::LIST_EXPR_A)});

  pl.push_back({PS::Terminal(T::SQR_OP),
        PS::NonTerminal(S::LIST_IND),
        PS::Terminal(T::SQR_CL),
        PS::NonTerminal(S::LIST_EXPR_A)});

  pl.push_back({});

  table_.emplace(S::LIST_EXPR_A, std::move(pl));
  pl.clear();

  //LIST_EXPR_B
  pl.push_back({PS::Terminal(T::IDN)});
  pl.push_back({PS::NonTerminal(S::F_LIT)});

  table_.emplace(S::LIST_EXPR_B, std::move(pl));
  pl.clear();

  //MUL_EXPR
  pl.push_back({PS::NonTerminal(S::LIST_EXPR),
        PS::NonTerminal(S::MUL_EXPR_A)});

  table_.emplace(S::MUL_EXPR, std::move(pl));
  pl.clear();

  //MUL_EXPR_A
  pl.push_back({PS::Terminal(T::MUL_OP),
        PS::NonTerminal(S::LIST_EXPR),
        PS::NonTerminal(S::MUL_EXPR_A)});

  pl.push_back({});

  table_.emplace(S::MUL_EXPR_A, std::move(pl));
  pl.clear();

  //ADD_EXPR
  pl.push_back({PS::NonTerminal(S::MUL_EXPR),
        PS::NonTerminal(S::ADD_EXPR_A)});

  table_.emplace(S::ADD_EXPR, std::move(pl));
  pl.clear();

  //ADD_EXPR_A
  pl.push_back({PS::Terminal(T::ADD_OP),
        PS::NonTerminal(S::MUL_EXPR),
        PS::NonTerminal(S::ADD_EXPR_A)});

  pl.push_back({});

  table_.emplace(S::ADD_EXPR_A, std::move(pl));
  pl.clear();

  //REL_EXPR
  pl.push_back({PS::NonTerminal(S::ADD_EXPR),
        PS::NonTerminal(S::REL_EXPR_A)});

  table_.emplace(S::REL_EXPR, std::move(pl));
  pl.clear();

  //REL_EXPR_A
  pl.push_back({PS::Terminal(T::REL_OP),
        PS::NonTerminal(S::ADD_EXPR)});

  pl.push_back({});

  table_.emplace(S::REL_EXPR_A, std::move(pl));
  pl.clear();

  //EQ_EXPR
  pl.push_back({PS::NonTerminal(S::REL_EXPR),
        PS::NonTerminal(S::EQ_EXPR_A)});

  table_.emplace(S::EQ_EXPR, std::move(pl));
  pl.clear();

  //EQ_EXPR_A
  pl.push_back({PS::Terminal(T::EQ_OP),
        PS::NonTerminal(S::REL_EXPR)});

  pl.push_back({});

  table_.emplace(S::EQ_EXPR_A, std::move(pl));
  pl.clear();

  //NOT_EXPR
  pl.push_back({PS::NonTerminal(S::EQ_EXPR)});

  pl.push_back({PS::Terminal(T::NOT),
        PS::NonTerminal(S::EQ_EXPR)});

  table_.emplace(S::NOT_EXPR, std::move(pl));
  pl.clear();

  //AND_EXPR
  pl.push_back({PS::NonTerminal(S::NOT_EXPR),
        PS::NonTerminal(S::AND_EXPR_A)});

  table_.emplace(S::AND_EXPR, std::move(pl));
  pl.clear();

  //AND_EXPR_A
  pl.push_back({PS::Terminal(T::AND),
        PS::NonTerminal(S::NOT_EXPR),
        PS::NonTerminal(S::AND_EXPR_A)});

  pl.push_back({});

  table_.emplace(S::AND_EXPR_A, std::move(pl));
  pl.clear();

  //OR_EXPR
  pl.push_back({PS::NonTerminal(S::AND_EXPR),
        PS::NonTerminal(S::OR_EXPR_A)});

  table_.emplace(S::OR_EXPR, std::move(pl));
  pl.clear();

  //OR_EXPR_A
  pl.push_back({PS::Terminal(T::OR),
        PS::NonTerminal(S::AND_EXPR),
        PS::NonTerminal(S::OR_EXPR_A)});

  pl.push_back({});

  table_.emplace(S::OR_EXPR_A, std::move(pl));
  pl.clear();

  //EXPR
  pl.push_back({PS::NonTerminal(S::OR_EXPR),
        PS::NonTerminal(S::EXPR_A)});

  table_.emplace(S::EXPR, std::move(pl));
  pl.clear();

  //EXPR_A
  pl.push_back({PS::Terminal(T::ASN),
        PS::NonTerminal(S::OR_EXPR)});

  pl.push_back({});

  table_.emplace(S::EXPR_A, std::move(pl));
} //initTable
