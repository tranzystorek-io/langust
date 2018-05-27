#ifndef EXPRESSIONLISTENER_HPP
#define EXPRESSIONLISTENER_HPP

#include "parsetree/BaseListener.hpp"

#include "parsetree/Node.hpp"
#include "ast/Node.hpp"
#include "ast/Ast.hpp"

#include <stack>
#include <forward_list>

namespace langust {
namespace parse {

struct AstNodeBox {
  ast::NodeObject::Type type;
  ast::Node* node;
  int elementsLeft;

  AstNodeBox(ast::NodeObject::Type t, ast::Node* n, int left = 0)
    : type(t), node(n), elementsLeft(left) {
  };
};

class ExpressionListener : public BaseListener {
public:
  using AstStack = std::stack<AstNodeBox>;
  using PTreeStack = std::stack<parse::Node*>;
public:
  ExpressionListener();

  void enterExpression(Node* node) override;

  ast::Ast getAst();

private:
  void processExpression();
  void process(Node& node);

  void unwindAstStack();

  void doExpr(Node& node);
  void doOrExpr(Node& node);
  void doAndExpr(Node& node);
  void doNotExpr(Node& node);
  void doEqExpr(Node& node);
  void doRelExpr(Node& node);
  void doAddExpr(Node& node);
  void doMulExpr(Node& node);
  void doListExpr(Node& node);
  void doListExprA(Node& node);

  void doAtom(Node& node);
  void doIdOrFapply(Node& node);
  void doInteger(Node& node);
  void doBoolean(Node& node);
  void doListLiteral(Node& node);
  void doFuncLiteral(Node& node);

private:
  AstStack astnodes_;
  PTreeStack ptreenodes_;

  bool awaitingExpr_;
};

} //parse
} //langust

#endif //EXPRESSIONLISTENER_HPP
