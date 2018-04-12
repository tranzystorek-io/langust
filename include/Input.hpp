#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

namespace langust {
namespace parse {

class Input {
public:
  Input(std::istream& in = std::cin);

  char currentChar() const;
  int getAbsPos() const;
  char getNextChar();
  char peekNextChar();

private:
  std::istream& in_;
  char current_;
};

} //parse
} //langust

#endif //INPUT_HPP
