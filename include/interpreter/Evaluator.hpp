#ifndef LANGUST_EVALUATOR_HPP
#define LANGUST_EVALUATOR_HPP

#include "MemoryArena.hpp"
#include "ast/Ast.hpp"
#include "ast/atom/FuncLiteral.hpp"

#include <stack>

namespace langust {
namespace interpreter {

struct AstEvalBox {
  using FuncPtr = std::shared_ptr<parse::ast::Node>*;

  parse::ast::NodeObject::Type type;
  parse::ast::Node* node;
  const std::shared_ptr<parse::ast::Node>* funcptr;
  bool evaluateNow;
  bool funcApplied;

  size_t operationIndex;
  size_t listIndex;

  AstEvalBox(const parse::ast::NodeObject& nobject)
    : type(nobject.type),
      node(nobject.getNode()),
      funcptr(nullptr),
      evaluateNow(false),
      funcApplied(false),
      operationIndex(0),
      listIndex(0) {
    if(type == parse::ast::NodeObject::FLIT)
      funcptr = &(nobject.object);
  }
};

class Evaluator {
public:
  using EvalStack = std::stack<AstEvalBox>;
  using ValueStack = std::stack<MemoryArena::Variable>;

public:
  Object eval(const parse::ast::Ast& tree);

private:
  void evaluateAssign(const EvalStack& stack, ValueStack& vstack);
  void evaluateOr(const EvalStack& stack, ValueStack& vstack);
  void evaluateAnd(const EvalStack& stack, ValueStack& vstack);
  void evaluateNot(const EvalStack& stack, ValueStack& vstack);
  void evaluateEq(const EvalStack& stack, ValueStack& vstack);
  void evaluateRel(const EvalStack& stack, ValueStack& vstack);
  void evaluateAdd(const EvalStack& stack, ValueStack& vstack);
  void evaluateMul(const EvalStack& stack, ValueStack& vstack);
  void evaluateList(EvalStack& stack, ValueStack& vstack);
  void evaluateId(const EvalStack& stack, ValueStack& vstack);
  void evaluateListLiteral(const EvalStack& stack, ValueStack& vstack);
  void evaluateFuncApply(EvalStack& stack, ValueStack& vstack);

  void fallbackOnRuntimeError(const std::string& errormsg);

private:
  MemoryArena arena_;
};

} //interpreter
} //langust

#endif //LANGUST_EVALUATOR_HPP
