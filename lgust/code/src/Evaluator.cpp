#include "interpreter/Evaluator.hpp"

#include "interpreter/RuntimeEvaluationException.hpp"

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

#include <iterator>
#include <stack>
#include <algorithm>

using namespace langust::interpreter;

Object Evaluator::eval(const parse::ast::Ast& tree) {
  using namespace parse;

  EvalStack stack; stack.emplace(tree.root_);
  ValueStack vstack;

  while(!stack.empty()) {
    AstEvalBox& box = stack.top();

    if(box.evaluateNow) {
      switch(box.type) {
      case ast::NodeObject::ASSIGN:
        evaluateAssign(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::OR:
        evaluateOr(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::AND:
        evaluateAnd(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::NOT:
        evaluateNot(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::EQ:
        evaluateEq(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::REL:
        evaluateRel(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::ADD:
        evaluateAdd(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::MUL:
        evaluateMul(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::LIST:
        evaluateList(stack, vstack);
        break;

      case ast::NodeObject::LLIT:
        evaluateListLiteral(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::FAPPLY:
        //this evaluator method handles stack itself
        evaluateFuncApply(stack, vstack);
        break;

      default:
        break;
      }
    }
    else {
      switch(box.type) {
      case ast::NodeObject::ASSIGN:
        {
          ast::Assign* ptr = dynamic_cast<ast::Assign*>(box.node);

          //check assignment validity
          if(ptr->lhs.type != parse::ast::NodeObject::ID)
            throw RuntimeEvaluationException("Left operand of assignment must be a variable identifier");

          stack.emplace(ptr->rhs);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::OR:
        {
          ast::Or* ptr = dynamic_cast<ast::Or*>(box.node);

          for(auto& nobj : ptr->operands)
            stack.emplace(nobj);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::AND:
        {
          ast::And* ptr = dynamic_cast<ast::And*>(box.node);

          for(auto& nobj : ptr->operands)
            stack.emplace(nobj);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::NOT:
        {
          ast::Not* ptr = dynamic_cast<ast::Not*>(box.node);

          stack.emplace(ptr->operand);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::EQ:
        {
          ast::Eq* ptr = dynamic_cast<ast::Eq*>(box.node);

          stack.emplace(ptr->left);
          stack.emplace(ptr->right);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::REL:
        {
          ast::Rel* ptr = dynamic_cast<ast::Rel*>(box.node);

          stack.emplace(ptr->left);
          stack.emplace(ptr->right);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::ADD:
        {
          ast::Add* ptr = dynamic_cast<ast::Add*>(box.node);

          for(auto& nobj : ptr->operands)
            stack.emplace(nobj);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::MUL:
        {
          ast::Mul* ptr = dynamic_cast<ast::Mul*>(box.node);

          for(auto& nobj : ptr->operands)
            stack.emplace(nobj);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::LIST:
        {
          ast::List* ptr = dynamic_cast<ast::List*>(box.node);

          stack.emplace(ptr->initial);

          for(auto& nobj : ptr->operands)
            stack.emplace(nobj);

          box.evaluateNow = true;
        }
        break;

      case ast::NodeObject::ID:
        evaluateId(stack, vstack);
        stack.pop();
        break;

      case ast::NodeObject::INT:
        {
          ast::Integer* ptr = dynamic_cast<ast::Integer*>(box.node);

          vstack.emplace(new Object(ptr->value));
          stack.pop();
        }
        break;

      case ast::NodeObject::BOOL:
        {
          ast::Boolean* ptr = dynamic_cast<ast::Boolean*>(box.node);

          vstack.emplace(new Object(ptr->value));
          stack.pop();
        }
        break;

      case ast::NodeObject::LLIT:
        {
          ast::ListLiteral* ptr = dynamic_cast<ast::ListLiteral*>(box.node);

          if(ptr->exprs.empty()) {
            vstack.emplace(new Object(new Object::List));
            stack.pop();
          }
          else {
            for(auto& nobj : ptr->exprs) {
              stack.emplace(nobj);
            }

            box.evaluateNow = true;
          }
        }
        break;

      case ast::NodeObject::FLIT:
        vstack.emplace(new Object(*box.funcptr));
        stack.pop();
        break;

      case ast::NodeObject::FAPPLY:
        {
          ast::FuncApply* ptr = dynamic_cast<ast::FuncApply*>(box.node);

          for(auto& nobj : ptr->exprs)
            stack.emplace(nobj);

          box.evaluateNow = true;
        }
        break;

      default:
        break;
      }
    }
  }

  return *(vstack.top());
}

void Evaluator::evaluateAssign(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::Assign* assignptr = dynamic_cast<parse::ast::Assign*>(box.node);

  const MemoryArena::Variable& rval = vstack.top();

  parse::ast::Identifier* idptr = dynamic_cast<parse::ast::Identifier*>(assignptr->lhs.getNode());
  const std::string& vname = idptr->str;

  if(arena_.isDefined(vname)) {
    arena_.at(vname) = rval;
  }
  else {
    arena_.put(vname, rval);
  }
}

void Evaluator::evaluateOr(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::Or* orptr = dynamic_cast<parse::ast::Or*>(box.node);

  bool result = false;
  const size_t noperands = orptr->operands.size();
  size_t i = 0;

  for(; i<noperands; ++i) {
    const MemoryArena::Variable& var = vstack.top();

    if(var->vtype != VarType::BOOL) {
      fallbackOnRuntimeError("Logical or operand must be a Boolean");
    }

    result = result || var->b;
    vstack.pop();

    if(result)
      break;
  }

  while(++i < noperands)
    vstack.pop();

  vstack.emplace(new Object(result));
}

void Evaluator::evaluateAnd(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::And* andptr = dynamic_cast<parse::ast::And*>(box.node);

  bool result = true;
  const size_t noperands = andptr->operands.size();
  size_t i = 0;

  for(; i<noperands; ++i) {
    const MemoryArena::Variable& var = vstack.top();

    if(var->vtype != VarType::BOOL) {
      fallbackOnRuntimeError("Logical and operand must be a Boolean");
    }

    result = result && var->b;
    vstack.pop();

    if(!result)
      break;
  }

  while(++i < noperands)
    vstack.pop();

  vstack.emplace(new Object(result));
}

void Evaluator::evaluateNot(const EvalStack& stack, ValueStack& vstack) {
  const MemoryArena::Variable& var = vstack.top();

  if(var->vtype != VarType::BOOL) {
    fallbackOnRuntimeError("Logical not operand must be a Boolean");
  }

  bool result = !(var->b);
  vstack.pop();

  vstack.emplace(new Object(result));
}

void Evaluator::evaluateEq(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::Eq* eqptr = dynamic_cast<parse::ast::Eq*>(box.node);

  MemoryArena::Variable leftv = vstack.top(); vstack.pop();
  MemoryArena::Variable rightv = vstack.top(); vstack.pop();

  switch(eqptr->type) {
  case parse::ast::Eq::EQU:
    vstack.emplace(new Object(*leftv == *rightv));
    break;

  case parse::ast::Eq::NEQ:
    vstack.emplace(new Object(*leftv != *rightv));
    break;
  }
}

void Evaluator::evaluateRel(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::Rel* relptr = dynamic_cast<parse::ast::Rel*>(box.node);

  MemoryArena::Variable leftv = vstack.top(); vstack.pop();
  const MemoryArena::Variable& rightv = vstack.top();

  if(leftv->vtype != VarType::INT || rightv->vtype != VarType::INT) {
    fallbackOnRuntimeError("Relation operand must be an Integer");
  }

  bool result;

  switch(relptr->type) {
  case parse::ast::Rel::LT:
    result = leftv->i < rightv->i;
    break;

  case parse::ast::Rel::LE:
    result = leftv->i <= rightv->i;
    break;

  case parse::ast::Rel::GT:
    result = leftv->i > rightv->i;
    break;

  case parse::ast::Rel::GE:
    result = leftv->i >= rightv->i;
    break;
  }

  vstack.pop();

  vstack.emplace(new Object(result));
}

void Evaluator::evaluateAdd(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::Add* addptr = dynamic_cast<parse::ast::Add*>(box.node);

  bool firstIteration = true;
  VarType optype;
  MemoryArena::Variable result;

  for(auto op : addptr->operators) {
    const MemoryArena::Variable& var = vstack.top();

    if(firstIteration) {
      optype = var->vtype;

      switch(optype) {
      case VarType::INT:
        result.reset(new Object(0));
        break;

      case VarType::LIST:
        result.reset(new Object(new Object::List));
        break;

      default:
        fallbackOnRuntimeError("Additive operand must be an Integer or a List");
      }

      firstIteration = false;
    }
    else {
      if(var->vtype != optype)
        fallbackOnRuntimeError("Additive operands must have uniform type");
    }

    switch(op) {
    case parse::ast::Add::ADD:
      switch(optype) {
      case VarType::INT:
        result->i += var->i;
        break;

      case VarType::LIST:
        result->list->insert(result->list->end(),
                             var->list->begin(), var->list->end());
        break;

      default:
        break;
      }
      break;

    case parse::ast::Add::SUB:
      if(optype != VarType::INT)
        fallbackOnRuntimeError("Subtraction can only be performed on Integers");
      result->i -= var->i;
      break;
    }

    vstack.pop();
  }

  vstack.push(result);
}

void Evaluator::evaluateMul(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::Mul* mulptr = dynamic_cast<parse::ast::Mul*>(box.node);

  int result = 1;

  for(auto op : mulptr->operators) {
    const MemoryArena::Variable& var = vstack.top();

    if(var->vtype != VarType::INT)
      fallbackOnRuntimeError("Multiplication operand must be an Integer");

    switch(op) {
    case parse::ast::Mul::MUL:
      result *= var->i;
      break;

    case parse::ast::Mul::DIV:
      result /= var->i;
      break;

    case parse::ast::Mul::MOD:
      result %= var-> i;
      break;
    }

    vstack.pop();
  }

  vstack.emplace(new Object(result));
}

void Evaluator::evaluateList(EvalStack& stack, ValueStack& vstack) {
  AstEvalBox& box = stack.top();

  parse::ast::List* listptr = dynamic_cast<parse::ast::List*>(box.node);

  MemoryArena::Variable result; //= vstack.top(); vstack.pop();

  // if(result->vtype != VarType::LIST) {
    // fallbackOnRuntimeError("List operation must have a List as first operand");
  // }

  parse::ast::List::Type op = listptr->operators[box.operationIndex];

  //MemoryArena::Variable var = vstack.top(); vstack.pop();

    switch(op) {
    case parse::ast::List::FLT:
      {
        MemoryArena::Variable modifiedElem;
        MemoryArena::Variable fvar;

        if(box.listIndex == 0) {
          result = vstack.top(); vstack.pop();
          fvar = vstack.top(); vstack.pop();

          if(fvar->vtype != VarType::FUNC)
            fallbackOnRuntimeError("Filter operator called on non-function object");
        }
        else {
          if(box.funcApplied) {
            arena_.exitFrame();
            modifiedElem = vstack.top(); vstack.pop();
            result = vstack.top(); vstack.pop();
            fvar = vstack.top(); vstack.pop();
          }
          else {
            result = vstack.top(); vstack.pop();
            fvar = vstack.top(); vstack.pop();
          }
        }

        if(result->vtype != VarType::LIST)
          fallbackOnRuntimeError("List operation must have a List as first operand");

        if(box.funcApplied) {
          if(modifiedElem->vtype != VarType::BOOL)
            fallbackOnRuntimeError("Filter operand function must return Boolean value");

          parse::ast::FuncLiteral* fptr = dynamic_cast<parse::ast::FuncLiteral*>(fvar->func.get());

          size_t size = fptr->ops.size();

          //pop all function byproducts
          for(size_t i=0; i<size;++i)
            vstack.pop();

          Object::List* newlist = new Object::List(result->list->begin(), result->list->end());

          if(!modifiedElem->b) {
            auto it = newlist->begin();
            std::advance(it, box.listIndex - 1);

            *it = Object();
            it->i = -1;
          }

          if(box.listIndex >= newlist->size()) {
            //collect all non-marked elements
            newlist->remove_if([](const Object& obj) {
                                 return obj.vtype == VarType::VOID && obj.i == -1;
                               });

            box.listIndex = 0;
            ++box.operationIndex;

            vstack.emplace(new Object(newlist));

            if(box.operationIndex >= listptr->operators.size())
              stack.pop();
          }
          else {
            vstack.push(fvar);
            vstack.emplace(new Object(newlist));
          }

          box.funcApplied = false;
        }
        else {
          vstack.push(fvar);
          vstack.push(result);

          parse::ast::FuncLiteral* fptr = dynamic_cast<parse::ast::FuncLiteral*>(fvar->func.get());

          if(fptr->params.size() != 1)
            fallbackOnRuntimeError("Filter operand function must take exactly one argument");

          if(result->list->size()) {

            auto it = result->list->begin();
            std::advance(it, box.listIndex);

            //set copy of target list element as func param
            arena_.enterFrame();
            arena_.put(fptr->params.front(), MemoryArena::Variable(new Object(*it)));

            if(fptr->ret)
              stack.emplace(fptr->ret);

            std::for_each(fptr->ops.rbegin(),
                          fptr->ops.rend(),
                          [&stack](const parse::ast::NodeObject& nobj) {
                            stack.emplace(nobj);
                          });

            ++box.listIndex;
            box.funcApplied = true;
          }
          else {
            box.listIndex = 0;
            ++box.operationIndex;

            if(box.operationIndex >= listptr->operators.size()) {
              stack.pop();
            }
          }
        }
      }
      break;

    case parse::ast::List::MAP:
      {
        //TODO MAP
        MemoryArena::Variable modifiedElem;
        MemoryArena::Variable fvar;

        if(box.listIndex == 0) {
          result = vstack.top(); vstack.pop();
          fvar = vstack.top(); vstack.pop();

          if(fvar->vtype != VarType::FUNC)
            fallbackOnRuntimeError("Filter operator called on non-function object");
        }
        else {
          if(box.funcApplied) {
            arena_.exitFrame();
            modifiedElem = vstack.top(); vstack.pop();
            result = vstack.top(); vstack.pop();
            fvar = vstack.top(); vstack.pop();
          }
          else {
            result = vstack.top(); vstack.pop();
            fvar = vstack.top(); vstack.pop();
          }
        }

        if(result->vtype != VarType::LIST)
          fallbackOnRuntimeError("List operation must have a List as first operand");

        if(box.funcApplied) {
          parse::ast::FuncLiteral* fptr = dynamic_cast<parse::ast::FuncLiteral*>(fvar->func.get());

          size_t size = fptr->ops.size();

          //pop all function byproducts
          for(size_t i=0; i<size;++i)
            vstack.pop();

          Object::List* newlist = new Object::List(result->list->begin(), result->list->end());

          // if(!modifiedElem->b) {
          //   auto it = newlist->begin();
          //   std::advance(it, box.listIndex - 1);

          //   *it = Object();
          //   it->i = -1;
          // }

          auto it = newlist->begin();
          std::advance(it, box.listIndex - 1);
          *it = *modifiedElem;

          if(box.listIndex >= newlist->size()) {
            box.listIndex = 0;
            ++box.operationIndex;

            vstack.emplace(new Object(newlist));

            if(box.operationIndex >= listptr->operators.size())
              stack.pop();
          }
          else {
            vstack.push(fvar);
            vstack.emplace(new Object(newlist));
          }

          box.funcApplied = false;
        }
        else {
          vstack.push(fvar);
          vstack.push(result);

          parse::ast::FuncLiteral* fptr = dynamic_cast<parse::ast::FuncLiteral*>(fvar->func.get());

          if(fptr->params.size() != 1)
            fallbackOnRuntimeError("Filter operand function must take exactly one argument");

          if(result->list->size()) {

            auto it = result->list->begin();
            std::advance(it, box.listIndex);

            //set copy of target list element as func param
            arena_.enterFrame();
            arena_.put(fptr->params.front(), MemoryArena::Variable(new Object(*it)));

            if(fptr->ret)
              stack.emplace(fptr->ret);

            std::for_each(fptr->ops.rbegin(),
                          fptr->ops.rend(),
                          [&stack](const parse::ast::NodeObject& nobj) {
                            stack.emplace(nobj);
                          });

            ++box.listIndex;
            box.funcApplied = true;
          }
          else {
            box.listIndex = 0;
            ++box.operationIndex;

            if(box.operationIndex >= listptr->operators.size()) {
              stack.pop();
            }
          }
        }
      }
      break;

    case parse::ast::List::ACCESS_ONE:
      {
        result = vstack.top(); vstack.pop();
        MemoryArena::Variable var = vstack.top(); vstack.pop();

        if(var->vtype != VarType::INT) {
          fallbackOnRuntimeError("List access operand must be an Integer");
        }

        size_t pos = var->i;

        if(pos < 0 || pos >= result->list->size()) {
          fallbackOnRuntimeError("List access out of range");
        }

        auto it = result->list->begin();
        advance(it, pos);
        auto itend = it;
        advance(itend, 1);

        Object::List* newlist = new Object::List;
        newlist->assign(it, itend);

        result = MemoryArena::Variable(new Object(newlist));
        ++box.operationIndex;
        vstack.push(result);

        if(box.operationIndex >= listptr->operators.size())
          stack.pop();
      }
      break;

    case parse::ast::List::ACCESS_FROM:
      {
        result = vstack.top(); vstack.pop();
        MemoryArena::Variable var = vstack.top(); vstack.pop();

        if(var->vtype != VarType::INT) {
          fallbackOnRuntimeError("List access operand must be an Integer");
        }

        size_t pos = var->i;

        if(pos < 0 || pos >= result->list->size()) {
          fallbackOnRuntimeError("List access out of range");
        }

        auto it = result->list->begin();
        advance(it, pos);

        Object::List* newlist = new Object::List;
        newlist->assign(it, result->list->end());

        result = MemoryArena::Variable(new Object(newlist));
        ++box.operationIndex;
        vstack.push(result);

        if(box.operationIndex >= listptr->operators.size())
          stack.pop();
      }
      break;

    case parse::ast::List::ACCESS_TO:
      {
        result = vstack.top(); vstack.pop();
        MemoryArena::Variable var = vstack.top(); vstack.pop();

        if(var->vtype != VarType::INT) {
          fallbackOnRuntimeError("List access operand must be an Integer");
        }

        size_t pos = var->i;

        if(pos < 0 || pos >= result->list->size()) {
          fallbackOnRuntimeError("List access out of range");
        }

        auto it = result->list->begin();
        advance(it, pos + 1);

        Object::List* newlist = new Object::List;
        newlist->assign(result->list->begin(), it);

        result = MemoryArena::Variable(new Object(newlist));
        ++box.operationIndex;
        vstack.push(result);

        if(box.operationIndex >= listptr->operators.size())
          stack.pop();
      }
      break;

    case parse::ast::List::ACCESS_RANGE:
      {
        result = vstack.top(); vstack.pop();
        MemoryArena::Variable var = vstack.top(); vstack.pop();
        MemoryArena::Variable range_end = vstack.top(); vstack.pop();

        if(var->vtype != VarType::INT || range_end->vtype != VarType::INT) {
          fallbackOnRuntimeError("List access operand must be an Integer");
        }

        size_t posbeg = var->i;
        size_t posend = range_end->i;
        size_t size = result->list->size();

        if(posbeg < 0 || posend < 0 || posbeg >= size || posend >= size) {
          fallbackOnRuntimeError("List access out of range");
        }

        if(posbeg > posend) {
          fallbackOnRuntimeError("Range access begin is situated after end");
        }

        auto itbeg = result->list->begin();
        advance(itbeg, posbeg);

        auto itend = itbeg;
        advance(itend, posend - posbeg + 1);

        Object::List* newlist = new Object::List;
        newlist->assign(itbeg, itend);

        result = MemoryArena::Variable(new Object(newlist));
        ++box.operationIndex;
        vstack.push(result);

        if(box.operationIndex >= listptr->operators.size())
          stack.pop();
      }
      break;
    }

  // if(box.operationIndex >= listptr->operators.size())
  //   stack.pop();

  // vstack.push(result);
}

void Evaluator::evaluateId(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::Identifier* idptr = dynamic_cast<parse::ast::Identifier*>(box.node);
  const std::string& vname = idptr->str;

  if(arena_.isDefined(vname)) {
    vstack.push(arena_.at(vname));
  }
  else {
    std::string msg("Variable identifier \"");
    msg += vname;
    msg += "\" is not defined in current scope";

    fallbackOnRuntimeError(msg);
  }
}

void Evaluator::evaluateListLiteral(const EvalStack& stack, ValueStack& vstack) {
  const AstEvalBox& box = stack.top();

  parse::ast::ListLiteral* lptr = dynamic_cast<parse::ast::ListLiteral*>(box.node);

  Object::List* list = new Object::List;
  const size_t nelems = lptr->exprs.size();

  for(size_t i=0; i<nelems; ++i) {
    const MemoryArena::Variable& var = vstack.top();

    list->push_back(*var);

    vstack.pop();
  }

  vstack.emplace(new Object(list));
}

void Evaluator::evaluateFuncApply(EvalStack& stack, ValueStack& vstack) {
  AstEvalBox& box = stack.top();

  parse::ast::FuncApply* ptr = dynamic_cast<parse::ast::FuncApply*>(box.node);

  if(box.funcApplied) {
    arena_.exitFrame();

    const MemoryArena::Variable& fvar = arena_.at(ptr->name);
    parse::ast::FuncLiteral* fptr = dynamic_cast<parse::ast::FuncLiteral*>(fvar->func.get());

    MemoryArena::Variable returnval;

    if(fptr->ret) {
      returnval = vstack.top();
      vstack.pop();
    }
    else {
      returnval.reset(new Object);
    }

    size_t size = fptr->ops.size();

    for(size_t i=0; i<size; ++i)
      vstack.pop();

    vstack.push(returnval);
    stack.pop();
  }
  else {
    const std::string& fname = ptr->name;

    if(!arena_.isDefined(fname)) {
      std::string msg = "Variable identifier \""
        + ptr->name
        + "\" is not defined in current scope";

      fallbackOnRuntimeError(msg);
    }

    const MemoryArena::Variable& fvar = arena_.at(fname);

    if(fvar->vtype != VarType::FUNC)
      fallbackOnRuntimeError("Function apply called on non-function object");

    parse::ast::FuncLiteral* fptr = dynamic_cast<parse::ast::FuncLiteral*>(fvar->func.get());

    size_t paramsize = fptr->params.size();
    size_t argsize = ptr->exprs.size();

    if(paramsize != argsize) {
      std::string msg = "Incorrect argument number for function; expected "
        + std::to_string(paramsize)
        + ", got " + std::to_string(argsize);

      fallbackOnRuntimeError(msg);
    }

    arena_.enterFrame();
    for(auto& pname : fptr->params) {
      arena_.put(pname, vstack.top());
      vstack.pop();
    }

    if(fptr->ret)
      stack.emplace(fptr->ret);

    std::for_each(fptr->ops.rbegin(),
                  fptr->ops.rend(),
                  [&stack](const parse::ast::NodeObject& nobj) {
                    stack.emplace(nobj);
                  });

    box.funcApplied = true;
  }
}

void Evaluator::fallbackOnRuntimeError(const std::string& errormsg) {
  while(!arena_.isStackEmpty())
    arena_.exitFrame();

  throw RuntimeEvaluationException(errormsg);
}
