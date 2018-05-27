#include "parsetree/ParseTreeBuilder.hpp"

#include "ProductionTable.hpp"

using namespace langust::parse;

ParseTree&& ParseTreeBuilder::getGenerated() {
  return std::move(generated_);
}

void ParseTreeBuilder::addNode(const Token& t) {
  if(stack_.empty()) {
  }
  else {
    unwindStack();
    Node& node = *(stack_.top().ptr);

    node.children.emplace_back(new Node(t));
  }
}

void ParseTreeBuilder::addNode(SymbolId id, int prod) {
  if(stack_.empty()) {
    generated_.root.reset(new Node(id, prod));

    stack_.emplace(generated_.root.get(),
                   ProductionTable::Instance()
                   .getProduction(id, prod).size());
  }
  else {
    unwindStack();
    Node& node = *(stack_.top().ptr);

    node.children.emplace_back(new Node(id, prod));

    stack_.emplace(node.children.back().get(),
                   ProductionTable::Instance()
                   .getProduction(id, prod).size());
  }
}

void ParseTreeBuilder::unwindStack() {
  while(!stack_.empty()) {
    const Node& node = *(stack_.top().ptr);
    size_t psize = stack_.top().prodsize;

    if(node.children.size() < psize) {
      break;
    }

    stack_.pop();
  }
}
