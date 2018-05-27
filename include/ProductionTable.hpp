#ifndef PRODUCTION_TABLE_HPP
#define PRODUCTION_TABLE_HPP

#include "Token.hpp"
#include "Symbol.hpp"

#include <unordered_map>
#include <vector>

namespace langust {
namespace parse {

struct ProdSymbol {
  enum Type {
    TERMINAL,
    NONTERMINAL
  } type;

  union {
    Token::Type tok;
    SymbolId sym;
  } value;

  ProdSymbol(Type t)
    : type(t) {
  }

  bool isTerminal() const {
    return type == Type::TERMINAL;
  }

  bool operator==(const ProdSymbol& other) const {
    if(type != other.type) {
      return false;
    }

    switch(type) {
    case Type::TERMINAL:
      return value.tok == other.value.tok;

    case Type::NONTERMINAL:
      return value.sym == other.value.sym;
    }
  }

  static ProdSymbol Terminal(Token::Type t) {
    ProdSymbol ret(Type::TERMINAL);
    ret.value.tok = t;

    return ret;
  }

  static ProdSymbol NonTerminal(SymbolId s) {
    ProdSymbol ret(Type::NONTERMINAL);
    ret.value.sym = s;

    return ret;
  }
};

class ProductionTable {
public:
  using Production = std::vector<ProdSymbol>;
  using ProdList = std::vector<Production>;
  using Table = std::unordered_map<SymbolId, ProdList>;

public:
  //ProductionTable();
  static ProductionTable& Instance();

  const Production& getProduction(SymbolId id, int index) const;

private:
  ProductionTable();
  void initTable();

private:
  Table table_;
};

} //parse
} //langust

#endif //PRODUCTION_TABLE_HPP
