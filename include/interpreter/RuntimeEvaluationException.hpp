#ifndef LANGUST_RUNTIME_EVALUATION_EXCEPTION_HPP
#define LANGUST_RUNTIME_EVALUATION_EXCEPTION_HPP

#include <stdexcept>

namespace langust {
namespace interpreter {

class RuntimeEvaluationException : public std::runtime_error {
public:
  RuntimeEvaluationException(const std::string& msg)
    : std::runtime_error(msg) {
  }
};

} //interpreter
} //langust

#endif //LANGUST_RUNTIME_EVALUATION_EXCEPTION_HPP
