#include "interpreter/Interpreter.hpp"

#include "interpreter/RuntimeEvaluationException.hpp"

#include "ast/atom/FuncLiteral.hpp"

#include "Symbol.hpp"

#include <functional>
#include <iostream>

#include <cstdlib>

using namespace langust::interpreter;

Interpreter::Interpreter()
  : //lbuf_(std::bind(&Interpreter::newLineCallback, std::ref(*this))),
    //is_(&lbuf_),
  input_(std::cin,
         std::bind(&Interpreter::newLineCallback, std::ref(*this)),
         std::bind(&Interpreter::eofCallback, std::ref(*this))),
  lexer_(input_),
  parser_(lexer_),
  newLine_(false),
  parsing_(false),
  line_(1) {
}

void Interpreter::newLineCallback() {
  if(input_.isInputSinceCallback()) {
    if(parsing_)
      std::cout << "$ " << std::flush;
    else
      parsing_ = true;

    newLine_ = true;
  }
  else {
    parsing_ = false;
    ++line_;
    std::cout << "(" << line_ << ")> " << std::flush;
  }
  // newLine_ = true;
  // std::cout << "NEWLINE DETECTED!" << std::endl;
}

void Interpreter::eofCallback() {
  std::cout << "Exiting..." << std::endl;
  exit(0);
}

void printObject(const Object& obj) {
  switch(obj.vtype) {
  case VarType::INT:
    std::cout << obj.i;
    break;

  case VarType::BOOL:
    std::cout << (obj.b ? "True" : "False");
    break;

  case VarType::LIST:
    std::cout << "[ ";
    {
      const size_t lastIndex = obj.list->size() - 1;
      size_t i=0;

      for(auto& lobj : *(obj.list)) {
        printObject(lobj);
        if(i++ == lastIndex)
          std::cout << " ";
        else
          std::cout << ", ";
      }
    }
    std::cout << "]";
    break;

  case VarType::FUNC:
    {
      langust::parse::ast::FuncLiteral* ptr = dynamic_cast<langust::parse::ast::FuncLiteral*>(obj.func.get());

      std::cout << "func/" << ptr->params.size();

      if(ptr->ret)
        std::cout << " with return";

      std::cout << std::endl;
    }
    break;

  case VarType::VOID:
    std::cout << "VoidObject";
    break;
  }
}

void Interpreter::execute() {
  std::string last;
  Object lastObj(0);

  while(1) {
    // if(newLine_) {
    //   std::cout << "(" << line << ")> " << std::flush;

    //   newLine_ = false;
    //   ++line;
    // }
    bool evaluationCorrect = false;
    std::cout << "(" << line_ << ")> " << std::flush;

    while(!newLine_) {
      parsing_ = true;

      parser_.processSymbol(parse::SymbolId::STMT);
        parse::ParseTree ptree = parser_.getTreeBuilder().getGenerated();
        ptree.walkWith(&astGenerator_);

        parse::ast::Ast ast = astGenerator_.getAst();
        // std::cout << ast.toString() << std::endl;
        // last = ast.toString();
        try {
          lastObj = evaluator_.eval(ast);
          evaluationCorrect = true;
        }
        catch(const RuntimeEvaluationException& ex) {
          std::cout << ex.what() << std::endl;
        }
      }

    // std::cout << last << std::endl;

    if(evaluationCorrect) {
      printObject(lastObj);
      std::cout << std::endl;
    }

    ++line_;
    newLine_ = false;
  }
}
