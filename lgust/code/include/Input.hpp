#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <functional>

namespace langust {
namespace parse {

class Input {
public:
  using Callback = std::function<void()>;

public:
  Input(std::istream& in = std::cin,
        const Callback& nlcallback = Callback(),
        const Callback& efocallback = Callback());

  char currentChar() const;
  int getAbsPos() const;
  char getNextChar();
  char peekNextChar();

  bool isInputSinceCallback() const;
  void ignore(int delim);

private:
  std::istream& in_;
  Callback newlineCallback_;
  Callback eofCallback_;
  char current_;
  bool inputSinceCallback_;
};

} //parse
} //langust

#endif //INPUT_HPP
