#ifndef TESTLISTENER_HPP
#define TESTLISTENER_HPP

#include "parsetree/BaseListener.hpp"

#include <iostream>

namespace langust {
namespace parse {

class TestListener : public BaseListener {
  void enterAtom(Node* node) override {
    std::cout << "ATOM/" << node->value.rule.production << std::endl;
  }
};

}
}

#endif //TESTLISTENER_HPP
