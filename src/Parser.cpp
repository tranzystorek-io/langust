#include "Parser.hpp"

#include "ParserTable.hpp"
#include "ProductionTable.hpp"

#include <iostream>
#include <algorithm>

using namespace langust::parse;

// Parser::Parser(std::istream& in, const Lexer::Callback& callback)
//   : lexer_(in, callback) {
// }

Parser::Parser(Lexer& lexer)
  : lexer_(lexer) {
}

void Parser::reset() {
  lexer_.ignoreLine();
  treebuilder_.reset();

  while(!stack_.empty()) {
    stack_.pop();
  }
}

bool langust::parse::Parser::isReady() const {
  return treebuilder_.isReady();
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
          //add token to the parse tree and get next from lexer

          treebuilder_.addNode(t);

          //finish parsing on semicolon
          if(stack_.empty() && t.type == Token::SCL) {
            //TODO this works only for STMT
            break;
          }

          //std::cout << "Getting new token from lexer..." << std::endl;
          t = lexer_.getToken();
        }//if ps.value.tok == t.type
        else {
          //TODO error
          std::cerr << "line: " << t.pos.line
                    << " col: " << t.pos.col
                    << "\nError: symbol \"" << t.str
                    << "\" doesn't match any parser rule."
                    << std::endl;

          reset();
          break;
        }
      }//if ps.isTerminal()
      else {
        Symbol s = ParserTable::Instance()
          .getSymbol(ps.value.sym, t.type);

        if(!s.isValid()) {
          reportUnexpectedToken(t, ps);

          reset();
          break;
        }//if !s.isValid()

#ifdef LANGUST_DEBUG
        std::cout << "token string: \"" << t.str
                  << "\", rule: " << Symbol::IdToString(ps.value.sym)
                  << ", production: " << s.rule.index + 1
                  << std::endl;
#endif //LANGUST_DEBUG

        treebuilder_.addNode(ps.value.sym, s.rule.index);
        pushProduction(ProductionTable::Instance()
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

      reset();
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
  const ParserTable::TokenList& tlist = ParserTable::Instance()
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

ParseTreeBuilder& Parser::getTreeBuilder() {
  return treebuilder_;
}
