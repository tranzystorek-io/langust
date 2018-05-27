#ifndef BASELISTENER_HPP
#define BASELISTENER_HPP

#include "Node.hpp"

namespace langust {
namespace parse {

struct BaseListener {
  virtual void enterAtom(Node* node) {
    (void)node;
  };

  virtual void enterExpression(Node* node) {
    (void)node;
  }
};

} //parse
} //langust

#endif //BASELISTENER_HPP
