#include "Input.hpp"

#include <limits>
#include <cctype>

using namespace langust::parse;

Input::Input(std::istream& in, const Callback& nlcallback, const Callback& eofcallback)
  : in_(in),
    newlineCallback_(nlcallback),
    eofCallback_(eofcallback),
    current_('\0'),
    inputSinceCallback_(false) {
  //set the input stream not to skip whitespaces
  in_ >> std::noskipws;
}

bool Input::isInputSinceCallback() const {
  return inputSinceCallback_;
}

char Input::currentChar() const {
  return current_;
}

int langust::parse::Input::getAbsPos() const {
  return in_.tellg();
}

char Input::getNextChar() {
  current_ = (char)in_.get();

  if(eofCallback_) {
    if(current_ == EOF) {
      eofCallback_();
    }
  }

  if(!std::isspace(current_)) {
    inputSinceCallback_ = true;
  }

  if(newlineCallback_)
    if(current_ == '\n') {
      newlineCallback_();
      inputSinceCallback_ = false;
    }

  return current_;
}

char Input::peekNextChar() {
  return in_.peek();
}

void Input::ignore(int delim) {
  in_.ignore(std::numeric_limits<std::streamsize>::max(), delim);
  current_ = '\0';
}
