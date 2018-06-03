#ifndef PARSETREE_HPP
#define PARSETREE_HPP

#include "parsetree/Node.hpp"
#include "parsetree/BaseListener.hpp"

namespace langust {
namespace parse {

struct ParseTree {
  Node::Ptr root;

  std::string toString() const;

  void walkWith(BaseListener* listener);
  void reset();
};

} //parse
} //langust

#endif
