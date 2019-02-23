#include <iostream>

#include <csignal>

#include "interpreter/Interpreter.hpp"

int main() {
  //std::cout << "Hello World!" << std::endl;

  //REPL is not interruptable
  std::signal(SIGINT, SIG_IGN);

  langust::interpreter::Interpreter interpreter;

  interpreter.execute();

  return 0;
}
