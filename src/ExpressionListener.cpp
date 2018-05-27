#include "parsetree/ExpressionListener.hpp"

#include "ast/Assign.hpp"
#include "ast/Or.hpp"
#include "ast/And.hpp"
#include "ast/Not.hpp"
#include "ast/Eq.hpp"
#include "ast/Rel.hpp"
#include "ast/Add.hpp"
#include "ast/Mul.hpp"
#include "ast/List.hpp"

#include "ast/atom/Integer.hpp"
#include "ast/atom/Boolean.hpp"
#include "ast/atom/Identifier.hpp"
#include "ast/atom/ListLiteral.hpp"
#include "ast/atom/FuncLiteral.hpp"
#include "ast/atom/FuncApply.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace langust::parse;

ExpressionListener::ExpressionListener()
  : awaitingExpr_(false) {
}

ast::Ast ExpressionListener::getAst() {
  if(astnodes_.size() > 1) {
    //TODO error
    std::cout << "AST tree is not ready yet!" << std::endl;
  }

  if(astnodes_.empty()) {
    //TODO error
  }

  AstNodeBox stacktop = astnodes_.top(); astnodes_.pop();

  return ast::Ast(stacktop.node, stacktop.type);
}

void ExpressionListener::enterExpression(Node* node) {
  //std::cout << "EXPR/" << node->value.rule.production << std::endl;

  ptreenodes_.push(node);
  awaitingExpr_ = false;

  processExpression();
}

void ExpressionListener::processExpression() {
  while(!ptreenodes_.empty()) {
    if(awaitingExpr_) {
      break;
    }

    Node* n = ptreenodes_.top(); ptreenodes_.pop();
    process(*n);
  }
}

void ExpressionListener::process(Node& node) {
  switch(node.value.rule.id) {
  case SymbolId::EXPR:
    doExpr(node);
    break;

  case SymbolId::OR_EXPR:
    doOrExpr(node);
    break;

  case SymbolId::AND_EXPR:
    doAndExpr(node);
    break;

  case SymbolId::NOT_EXPR:
    doNotExpr(node);
    break;

  case SymbolId::EQ_EXPR:
    doEqExpr(node);
    break;

  case SymbolId::REL_EXPR:
    doRelExpr(node);
    break;

  case SymbolId::ADD_EXPR:
    doAddExpr(node);
    break;

  case SymbolId::MUL_EXPR:
    doMulExpr(node);
    break;

  case SymbolId::LIST_EXPR:
    doListExpr(node);
    break;

  case SymbolId::LIST_EXPR_A:
    doListExprA(node);
    break;

  case SymbolId::LIST_IND:
  case SymbolId::LIST_IND_A:
  case SymbolId::LIST_IND_B:
    awaitingExpr_ = true;
    break;

  case SymbolId::ATOM:
    doAtom(node);
    break;

  default:
    break;
  }
}

void astInsertIntoAssign(ast::Assign* assign, int& left, const AstNodeBox& box) {
  if(!assign->lhs) {
    assign->lhs.object.reset(box.node);
    assign->lhs.type = box.type;
  }
  else {
    assign->rhs.object.reset(box.node);
    assign->rhs.type = box.type;
  }

  --left;
}

void astInsertIntoOr(ast::Or* orr, int& left, const AstNodeBox& box) {
  orr->operands.emplace_back(box.node, box.type);

  --left;
}

void astInsertIntoAnd(ast::And* andd, int& left, const AstNodeBox& box) {
  andd->operands.emplace_back(box.node, box.type);

  --left;
}

void astInsertIntoNot(ast::Not* nott, int& left, const AstNodeBox& box) {
  nott->operand.object.reset(box.node);
  nott->operand.type = box.type;

  --left;
}

void astInsertIntoEq(ast::Eq* eq, int& left, const AstNodeBox& box) {
  if(!eq->left) {
    eq->left.object.reset(box.node);
    eq->left.type = box.type;
  }
  else {
    eq->right.object.reset(box.node);
    eq->right.type = box.type;
  }

  --left;
}

void astInsertIntoRel(ast::Rel* rel, int& left, const AstNodeBox& box) {
  if(!rel->left) {
    rel->left.object.reset(box.node);
    rel->left.type = box.type;
  }
  else {
    rel->right.object.reset(box.node);
    rel->right.type = box.type;
  }

  --left;
}

void astInsertIntoAdd(ast::Add* add, int& left, const AstNodeBox& box) {
  add->operands.emplace_back(box.node, box.type);

  --left;
}

void astInsertIntoMul(ast::Mul* mul, int& left, const AstNodeBox& box) {
  mul->operands.emplace_back(box.node, box.type);

  --left;
}

void astInsertIntoList(ast::List* list, int& left, const AstNodeBox& box) {
  if(!list->initial) {
    list->initial.object.reset(box.node);
    list->initial.type = box.type;
  }
  else
    list->operands.emplace_back(box.node, box.type);

  --left;
}

void astInsertIntoListLiteral(ast::ListLiteral* llit, int& left, const AstNodeBox& box) {
  llit->exprs.emplace_back(box.node, box.type);

  --left;
}

void astInsertIntoFuncLiteral(ast::FuncLiteral* flit, int& left, const AstNodeBox& box) {
  if(left == 1) {
    if(flit->ret.type == ast::NodeObject::NONE)
      flit->ops.emplace_back(box.node, box.type);
    else {
      flit->ret.object.reset(box.node);
      flit->ret.type = box.type;
    }
  }
  else
    flit->ops.emplace_back(box.node, box.type);

  --left;
}

void astInsertIntoFuncApply(ast::FuncApply* fapply, int& left, const AstNodeBox& box) {
  fapply->exprs.emplace_back(box.node, box.type);

  --left;
}

void ExpressionListener::unwindAstStack() {
  while(astnodes_.size() > 1) {
    AstNodeBox box = astnodes_.top(); astnodes_.pop();
    AstNodeBox& parentNodeBox = astnodes_.top();

    switch(parentNodeBox.type) {
    case ast::NodeObject::ASSIGN:
      astInsertIntoAssign(dynamic_cast<ast::Assign*>(parentNodeBox.node),
                          parentNodeBox.elementsLeft,
                          box);
      break;

    case ast::NodeObject::OR:
      astInsertIntoOr(dynamic_cast<ast::Or*>(parentNodeBox.node),
                      parentNodeBox.elementsLeft,
                      box);
      break;

    case ast::NodeObject::AND:
      astInsertIntoAnd(dynamic_cast<ast::And*>(parentNodeBox.node),
                       parentNodeBox.elementsLeft,
                       box);
      break;

    case ast::NodeObject::NOT:
      astInsertIntoNot(dynamic_cast<ast::Not*>(parentNodeBox.node),
                          parentNodeBox.elementsLeft,
                          box);
      break;

    case ast::NodeObject::EQ:
      astInsertIntoEq(dynamic_cast<ast::Eq*>(parentNodeBox.node),
                      parentNodeBox.elementsLeft,
                      box);
      break;

    case ast::NodeObject::REL:
      astInsertIntoRel(dynamic_cast<ast::Rel*>(parentNodeBox.node),
                       parentNodeBox.elementsLeft,
                       box);
      break;

    case ast::NodeObject::ADD:
      astInsertIntoAdd(dynamic_cast<ast::Add*>(parentNodeBox.node),
                       parentNodeBox.elementsLeft,
                       box);
      break;

    case ast::NodeObject::MUL:
      astInsertIntoMul(dynamic_cast<ast::Mul*>(parentNodeBox.node),
                       parentNodeBox.elementsLeft,
                       box);
      break;

    case ast::NodeObject::LIST:
      astInsertIntoList(dynamic_cast<ast::List*>(parentNodeBox.node),
                        parentNodeBox.elementsLeft,
                        box);
      break;

    case ast::NodeObject::LLIT:
      astInsertIntoListLiteral(dynamic_cast<ast::ListLiteral*>(parentNodeBox.node),
                        parentNodeBox.elementsLeft,
                        box);
      if(parentNodeBox.elementsLeft)
        awaitingExpr_ = true;
      break;

    case ast::NodeObject::FLIT:
      astInsertIntoFuncLiteral(dynamic_cast<ast::FuncLiteral*>(parentNodeBox.node),
                        parentNodeBox.elementsLeft,
                        box);
      if(parentNodeBox.elementsLeft)
        awaitingExpr_ = true;
      break;

    case ast::NodeObject::FAPPLY:
      astInsertIntoFuncApply(dynamic_cast<ast::FuncApply*>(parentNodeBox.node),
                        parentNodeBox.elementsLeft,
                        box);
      if(parentNodeBox.elementsLeft)
        awaitingExpr_ = true;
      break;

    default:
      break;
    }

    if(parentNodeBox.elementsLeft < 0) {
      //TODO error
    }

    if(parentNodeBox.elementsLeft)
      break;
  }
}

void ExpressionListener::doExpr(Node& node) {
  //EXPR_A
  const Node* n = node.children[1].get();

  //assignment
  if(n->value.rule.production == 0) {
    astnodes_.emplace(ast::NodeObject::ASSIGN,
                      new ast::Assign, 2);

    ptreenodes_.push(n->children[1].get());
  }

  ptreenodes_.push(node.children[0].get());
}

void ExpressionListener::doOrExpr(Node& node) {
  //OR_EXPR_A
  const Node* n = node.children[1].get();

  std::vector<Node*> operands;

  //get operands
  while(n->value.rule.production == 0) {
    operands.push_back(n->children[1].get());
    n = n->children[2].get();
  }

  //or expr
  if(!operands.empty()) {
    astnodes_.emplace(ast::NodeObject::OR,
                      new ast::Or,
                      operands.size() + 1);

    std::for_each(operands.rbegin(),
                  operands.rend(),
                  [this](Node* ptr) {
                    ptreenodes_.push(ptr);
                  });
  }

  ptreenodes_.push(node.children[0].get());
}

void ExpressionListener::doAndExpr(Node& node) {
  //AND_EXPR_A
  const Node* n = node.children[1].get();

  std::vector<Node*> operands;

  //get operands
  while(n->value.rule.production == 0) {
    operands.push_back(n->children[1].get());
    n = n->children[2].get();
  }

  if(!operands.empty()) {
    astnodes_.emplace(ast::NodeObject::AND,
                      new ast::And,
                      operands.size() + 1);

    std::for_each(operands.rbegin(),
                  operands.rend(),
                  [this](Node* ptr) {
                    ptreenodes_.push(ptr);
                  });
  }

  ptreenodes_.push(node.children[0].get());
}

void ExpressionListener::doNotExpr(Node& node) {
  //not expr
  if(node.value.rule.production == 1) {
    astnodes_.emplace(ast::NodeObject::NOT,
                      new ast::Not, 1);
    ptreenodes_.push(node.children[1].get());
  }
  else {
    ptreenodes_.push(node.children[0].get());
  }
}

void ExpressionListener::doEqExpr(Node& node) {
  //EQ_EXPR_A
  const Node* n = node.children[1].get();

  //eq expr
  if(n->value.rule.production == 0) {
    ast::Eq::Type op;
    switch(n->children[0]->value.tok.value.eq_t) {
    case EqOpType::EQU:
      op = ast::Eq::EQU;
      break;

    case EqOpType::NEQ:
      op = ast::Eq::NEQ;
      break;
    }

    astnodes_.emplace(ast::NodeObject::EQ,
                      new ast::Eq(op), 2);
    ptreenodes_.push(n->children[1].get());
  }

  ptreenodes_.push(node.children[0].get());
}

void ExpressionListener::doRelExpr(Node& node) {
  //REL_EXPR_A
  const Node* n = node.children[1].get();

  //rel expr
  if(n->value.rule.production == 0) {
    ast::Rel::Type op;
    switch(n->children[0]->value.tok.value.rel_t) {
    case RelOpType::LT:
      op = ast::Rel::LT;
      break;

    case RelOpType::LE:
      op = ast::Rel::LE;
      break;

    case RelOpType::GT:
      op = ast::Rel::GT;
      break;

    case RelOpType::GE:
      op = ast::Rel::GE;
      break;
    }

    astnodes_.emplace(ast::NodeObject::REL,
                      new ast::Rel(op), 2);
    ptreenodes_.push(n->children[1].get());
  }

  ptreenodes_.push(node.children[0].get());
}

void ExpressionListener::doAddExpr(Node& node) {
  //ADD_EXPR_A
  const Node* n = node.children[1].get();

  std::vector<Node*> operands;
  std::vector<ast::Add::Type> operators = {ast::Add::ADD};

  while(n->value.rule.production == 0) {
    switch(n->children[0]->value.tok.value.add_t) {
    case AddOpType::ADD:
      operators.push_back(ast::Add::ADD);
      break;

    case AddOpType::SUB:
      operators.push_back(ast::Add::SUB);
      break;
    }

    operands.push_back(n->children[1].get());
    n = n->children[2].get();
  }

  //add expr
  if(!operands.empty()) {
    astnodes_.emplace(ast::NodeObject::ADD,
                      new ast::Add(operators),
                      operands.size() + 1);

    std::for_each(operands.rbegin(),
                  operands.rend(),
                  [this](Node* ptr) {
                    ptreenodes_.push(ptr);
                  });
  }

  ptreenodes_.push(node.children[0].get());
}

void ExpressionListener::doMulExpr(Node& node) {
  //MUL_EXPR_A
  const Node* n = node.children[1].get();

  std::vector<Node*> operands;
  std::vector<ast::Mul::Type> operators = {ast::Mul::MUL};

  while(n->value.rule.production == 0) {
    switch(n->children[0]->value.tok.value.mul_t) {
    case MulOpType::MUL:
      operators.push_back(ast::Mul::MUL);
      break;

    case MulOpType::DIV:
      operators.push_back(ast::Mul::DIV);
      break;

    case MulOpType::MOD:
      operators.push_back(ast::Mul::MOD);
      break;
    }

    operands.push_back(n->children[1].get());
    n = n->children[2].get();
  }

  //mul expr
  if(!operands.empty()) {
    astnodes_.emplace(ast::NodeObject::MUL,
                      new ast::Mul(operators),
                      operands.size() + 1);

    std::for_each(operands.rbegin(),
                  operands.rend(),
                  [this](Node* ptr) {
                    ptreenodes_.push(ptr);
                  });
  }

  ptreenodes_.push(node.children[0].get());
}

//given a ListInd, determine access type
ast::List::Type getListAccessType(Node& n) {
  if(n.value.rule.production == 1) //[:X]
    return ast::List::ACCESS_TO;

  const Node* listIndANode = n.children[1].get();

  if(listIndANode->value.rule.production == 1) //[X]
    return ast::List::ACCESS_ONE;

  const Node* listIndBNode = listIndANode->children[1].get();

  if(listIndBNode->value.rule.production == 1) //[X:]
    return ast::List::ACCESS_FROM;

  return ast::List::ACCESS_RANGE; //[X:Y]
}

void ExpressionListener::doListExpr(Node& node) {
  //LIST_EXPR_A
  Node* n = node.children[1].get();

  std::vector<Node*> elements;
  std::vector<ast::List::Type> operators;
  int nargs = 1;

  while(n->value.rule.production < 2) {
    int pnumber = n->value.rule.production;

    elements.push_back(n);

    switch(pnumber) {
    case 0: //pipe operator
      switch(n->children[0]->value.tok.value.pipe_t) {
      case PipeOpType::FLT:
        operators.push_back(ast::List::FLT);
        break;

      case PipeOpType::MAP:
        operators.push_back(ast::List::MAP);
        break;
      }
      ++nargs;
      n = n->children[2].get();
      break;

    case 1: //list index
      {
        ast::List::Type type = getListAccessType(*(n->children[1]));
        switch(type) {
        case ast::List::ACCESS_RANGE:
          nargs += 2;
          break;

        default:
          ++nargs;
          break;
        }

        operators.push_back(type);
      }
      n = n->children[3].get();
      break;
    }//switch
  }

  //list expr
  if(!elements.empty()) {
    //TODO count number of needed expressions
    astnodes_.emplace(ast::NodeObject::LIST,
                      new ast::List(operators),
                      nargs);

    std::for_each(elements.rbegin(),
                  elements.rend(),
                  [this](Node* ptr) {
                    ptreenodes_.push(ptr);
                  });
  }

  ptreenodes_.push(node.children[0].get());
}

void ExpressionListener::doListExprA(Node& node) {
  if(node.value.rule.production == 0) {
    Node& listBNode = *(node.children[1]);

    switch(listBNode.value.rule.production) {
    case 0: //Identifier
      astnodes_.emplace(ast::NodeObject::ID,
                        new ast::Identifier(listBNode.children[0]->value.tok.str));
      unwindAstStack();
      break;

    case 1: //Func Literal
      doFuncLiteral(*(listBNode.children[0]));
      break;
    }
  }
  else {
    Node* listIndNode = node.children[1].get();

    ptreenodes_.push(listIndNode);

    if(listIndNode->value.rule.production == 0) {
      Node* listIndANode = listIndNode->children[1].get();

      if(listIndANode->value.rule.production == 0) {
        Node* listIndBNode = listIndANode->children[1].get();

        if(listIndBNode->value.rule.production == 0)
          ptreenodes_.push(listIndBNode);
      }
    }
  }
}

void ExpressionListener::doAtom(Node& node) {
  switch(node.value.rule.production) {
  case 0: //Integer
    {
      Node& intNode = *(node.children[0]);

      doInteger(intNode);
    }
    unwindAstStack();
    break;

  case 1: //Boolean
    {
      Node& boolNode = *(node.children[0]);

      doBoolean(boolNode);
    }
    unwindAstStack();
    break;

  case 2: //List Literal
    {
      Node& listLitNode = *(node.children[0]);

      doListLiteral(listLitNode);
    }
    break;

  case 3: //Func Literal
    {
      Node& funcLitNode = *(node.children[0]);

      doFuncLiteral(funcLitNode);
    }
    break;

  case 4: //Id Or Func Apply
    {
      Node& idOrFapplyNode = *(node.children[0]);

      doIdOrFapply(idOrFapplyNode);
    }
    break;

  case 5: //Parenthesised Expression
    awaitingExpr_ = true;
    break;
  }
}

//given an ArgLis, counts its arguments
int countArgs(Node& n) {
  int ret = 1;
  const Node* argNode = n.children[1].get();

  while(argNode->value.rule.production == 0) {
    ++ret;
    argNode = argNode->children[2].get();
  }

  return ret;
}

void ExpressionListener::doIdOrFapply(Node& node) {
  Node& idNode = *(node.children[0]);
  Node& orFapplyNode = *(node.children[1]);

  switch(orFapplyNode.value.rule.production) {
  case 0: //Func Apply
    {
      Node& orFapplyANode = *(orFapplyNode.children[2]);
      int nargs = 0;

      if(orFapplyANode.value.rule.production == 0) {
        nargs = countArgs(*(orFapplyANode.children[0]));
        awaitingExpr_ = true;
      }

      astnodes_.emplace(ast::NodeObject::FAPPLY,
                        new ast::FuncApply(idNode.value.tok.str),
                        nargs);
    }
    if(!awaitingExpr_)
      unwindAstStack();
    break;

  case 1: //Identifier Only
    astnodes_.emplace(ast::NodeObject::ID,
                      new ast::Identifier(idNode.value.tok.str));
    unwindAstStack();
    break;
  }
}

void ExpressionListener::doInteger(Node& node) {
  astnodes_.emplace(ast::NodeObject::INT,
                    new ast::Integer(node.value.tok
                                     .value.i));
}

void ExpressionListener::doBoolean(Node& node) {
  astnodes_.emplace(ast::NodeObject::BOOL,
                    new ast::Boolean(node.value.tok
                                     .value.b));
}

void ExpressionListener::doListLiteral(Node& node) {
  Node& lisLitANode = *(node.children[1]);
  int nargs = 0;

  if(lisLitANode.value.rule.production == 0) {
    nargs = countArgs(*(lisLitANode.children[0]));
    awaitingExpr_ = true;
  }

  astnodes_.emplace(ast::NodeObject::LLIT,
                    new ast::ListLiteral(),
                    nargs);

  if(!awaitingExpr_)
    unwindAstStack();
}

std::vector<std::string> getParamList(Node& n) {
  std::vector<std::string> ret;

  ret.push_back(n.children[0]->value.tok.str);

  const Node* paramListANode = n.children[1].get();

  while(paramListANode->value.rule.production == 0) {
    ret.push_back(paramListANode->children[1]->value.tok.str);
    paramListANode = paramListANode->children[2].get();
  }

  return ret;
}

//given an SList, count its statements
int countStatements(Node& n) {
  int ret = 1;

  const Node* SListANode = n.children[1].get();

  while(SListANode->value.rule.production == 0) {
    ++ret;
    SListANode = SListANode->children[1].get();
  }

  return ret;
}

void ExpressionListener::doFuncLiteral(Node& node) {
  Node& flitContNode = *(node.children[2]);
  std::vector<std::string> params;
  int contAIndex = 2;

  if(flitContNode.value.rule.production == 0) {
    params = getParamList(*(flitContNode.children[0]));

    contAIndex = 3;
  }

  Node& flitContANode = *(flitContNode.children[contAIndex]);
  int nstatements = 0;
  int contBIndex = 0;

  if(flitContANode.value.rule.production == 0) {
    nstatements = countStatements(*(flitContANode.children[0]));
    contBIndex = 1;
    awaitingExpr_ = true;
  }

  Node& flitContBNode = *(flitContANode.children[contBIndex]);
  bool hasReturnStatement = false;

  if(flitContBNode.value.rule.production == 0) {
    ++nstatements;
    awaitingExpr_ = true;
    hasReturnStatement = true;
  }

  astnodes_.emplace(ast::NodeObject::FLIT,
                    new ast::FuncLiteral(params),
                    nstatements);

  if(hasReturnStatement) {
    dynamic_cast<ast::FuncLiteral*>(astnodes_.top().node)->ret.type = ast::NodeObject::ASSIGN;
  }

  if(!awaitingExpr_)
    unwindAstStack();
}

