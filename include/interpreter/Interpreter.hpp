#ifndef LANGUST_INTERPRETER_HPP
#define LANGUST_INTERPRETER_HPP

#include <iostream>

#include "Parser.hpp"
#include "interpreter/Evaluator.hpp"
#include "parsetree/ExpressionListener.hpp"

namespace langust {
namespace interpreter {

class Interpreter {
public:
  Interpreter();

  void execute();

private:
  //special callbacks from Input
  void newLineCallback();
  void eofCallback();

private:
  parse::Input input_;
  parse::Lexer lexer_;
  parse::Parser parser_;
  parse::ExpressionListener astGenerator_;

  Evaluator evaluator_;

  bool newLine_;
  bool parsing_;

  int line_;
};

} //interpreter
} //langust

#endif //LANGUST_INTERPRETER
