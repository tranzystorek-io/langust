#include "parsetree/ParseTree.hpp"

#include <sstream>
#include <stack>
#include <algorithm>

using namespace langust::parse;

std::string ParseTree::toString() const {
  std::ostringstream oss;

  std::stack<Node*> stack;
  stack.push(root.get());

  while(!stack.empty()) {
    if(!stack.top()) {
      stack.pop();
      oss << ") ";
      continue;
    }

    const Node& n = *(stack.top()); stack.pop();

    switch(n.type) {
    case Node::TERMINAL:
      oss << '\"' << n.value.tok.str << "\" ";
      break;

    case Node::NONTERMINAL:
      oss << '(' << Symbol::IdToString(n.value.rule.id)
          << '/' << n.value.rule.production << ": ";

      stack.push(nullptr);
      break;
    }

    std::for_each(n.children.rbegin(),
                  n.children.rend(),
                  [&stack](const Node::Ptr& ptr){
                    stack.push(ptr.get());
                  });
  }

  return oss.str();
}

void langust::parse::ParseTree::walkWith(BaseListener* listener) {
  std::stack<Node*> stack;
  stack.push(root.get());

  while(!stack.empty()) {
    Node* n = stack.top(); stack.pop();

    if(n->type == Node::NONTERMINAL) {
      switch(n->value.rule.id) {
      case SymbolId::ATOM:
        listener->enterAtom(n);
        break;

      case SymbolId::EXPR:
        listener->enterExpression(n);
        break;

      default:
        break;
      }
    }

    std::for_each(n->children.rbegin(),
                  n->children.rend(),
                  [&stack](const Node::Ptr& ptr) {
                    stack.push(ptr.get());
                  });
  }
}
