#include "Parser.hpp"

#include <iostream>
#include <algorithm>

using namespace langust::parse;

Parser::Parser(std::istream& in)
  : lexer_(in) {
}

void Parser::processSymbol(SymbolId sym) {
  using Production = ProductionTable::Production;

  stack_.push(ProdSymbol::NonTerminal(sym));
  Token t = lexer_.getToken();

  while(!stack_.empty()) {
    ProdSymbol ps = stack_.top(); stack_.pop();

#ifdef LANGUST_DEBUG
    bool is_term = ps.isTerminal();
    std::cerr << "\n## prod symbol "
              << (is_term
                  ? "(token): "
                  : "(production): ")
              << (is_term
                  ? Token::TypeToString(ps.value.tok)
                  : Symbol::IdToString(ps.value.sym))
              << std::endl;
#endif //LANGUST_DEBUG

    if(t.isValid()) {
      if(ps.isTerminal()) {
        if(ps.value.tok == t.type) {
          t = lexer_.getToken();
        }//if ps.value.tok == t.type
        else {
          //TODO error
          std::cerr << "line: " << t.pos.line
                    << " col: " << t.pos.col
                    << "\nError: symbol \"" << t.str
                    << "\" doesn't match any parser rule."
                    << std::endl;

          break;
        }
      }//if ps.isTerminal()
      else {
        Symbol s = partable_.getSymbol(ps.value.sym, t.type);

        if(!s.isValid()) {
          reportUnexpectedToken(t, ps);

          break;
        }//if !s.isValid()

#ifdef LANGUST_DEBUG
        std::cerr << "token string: \"" << t.str
                  << "\", rule: " << Symbol::IdToString(ps.value.sym)
                  << ", production: " << s.rule.index + 1
                  << std::endl;
#endif //LANGUST_DEBUG

        // const Production& prod = prodtable_
        //   .getProduction(ps.value.sym, s.rule.index);

        pushProduction(prodtable_
                       .getProduction(ps.value.sym,
                                      s.rule.index));
      }//else
    }//if t.isValid()
    else {
      //TODO error
      std::cerr << "line: " << t.pos.line
                << " col: " << t.pos.col
                << "\nError: invalid symbol \""
                << t.str << "\""
                << std::endl;

      break;
    }
  }//while
}//processSymbol

void Parser::pushProduction(const ProductionTable::Production& prod) {
  //push production symbols to stack (right to left)
  std::for_each(prod.rbegin(), prod.rend(),
                [this](const ProdSymbol& psym) {
                  stack_.push(psym);
                });
}

void Parser::reportUnexpectedToken(const Token& t, ProdSymbol ps) const {
  const ParserTable::TokenList& tlist = partable_
    .getTokensForSymbol(ps.value.sym);

  //report Unexpected Token error
  std::cerr << "line: " << t.pos.line
            << " col: " << t.pos.col
            << "\nUnexpected token: \"" << t.str
            << "\"\nDid you mean ";

  for(auto& tok : tlist) {
    std::cerr << Token::TypeToString(tok) << " ";
  }

  std::cerr << "?" << std::endl;
}
