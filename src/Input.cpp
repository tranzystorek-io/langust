#include "Input.hpp"

using namespace langust::parse;

Input::Input(std::istream& in)
  : in_(in),
    current_('\0') {
}

char Input::currentChar() const {
  return current_;
}

int langust::parse::Input::getAbsPos() const {
  return in_.tellg();
}

char Input::getNextChar() {
  return current_ = (char)in_.get();
}

char Input::peekNextChar() {
  return in_.peek();
}
