#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <string>
#include <memory>

namespace langust {
namespace parse {
namespace ast {

class Node {
public:
  virtual ~Node() {
  }

  //virtual std::string toString() const = 0;
};

struct NodeObject {
  enum Type {
    //EXPRS
    ASSIGN,
    OR, AND, NOT,
    EQ, REL,
    ADD,
    MUL,
    LIST,
    //ATOMS
    ID,
    INT, BOOL,
    LLIT,
    FLIT,
    FAPPLY
    , NONE
  } type;

  std::unique_ptr<Node> object;

  NodeObject()
    : type(Type::NONE) {
  }

  NodeObject(Node* ptr, Type t)
    : type(t),
      object(ptr) {
  }

  operator bool() const {
    return (bool)object;
  }
};

} //ast
} //parse
} //langust

#endif //AST_NODE_HPP
