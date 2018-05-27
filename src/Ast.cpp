#include "ast/Ast.hpp"

#include "ast/Assign.hpp"
#include "ast/Or.hpp"
#include "ast/And.hpp"
#include "ast/Not.hpp"
#include "ast/Eq.hpp"
#include "ast/Rel.hpp"
#include "ast/Add.hpp"
#include "ast/Mul.hpp"
#include "ast/List.hpp"

#include "ast/atom/Integer.hpp"
#include "ast/atom/Boolean.hpp"
#include "ast/atom/Identifier.hpp"
#include "ast/atom/ListLiteral.hpp"
#include "ast/atom/FuncLiteral.hpp"
#include "ast/atom/FuncApply.hpp"

#include <stack>
#include <algorithm>

using namespace langust::parse::ast;

std::string Ast::toString() const {
  std::string ret;

  struct NodeBox {
    Node* node;
    NodeObject::Type type;

    NodeBox(Node* n, NodeObject::Type t)
      : node(n),
        type(t) {
    }
  };

  std::stack<NodeBox> bfs; bfs.emplace(root_.object.get(), root_.type);

  while(!bfs.empty()) {
    NodeBox current = bfs.top(); bfs.pop();

    switch(current.type) {
    case NodeObject::ASSIGN:
      ret += "ASSIGN( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        Assign* ptr = dynamic_cast<Assign*>(current.node);

        bfs.emplace(ptr->rhs.object.get(), ptr->rhs.type);
        bfs.emplace(ptr->lhs.object.get(), ptr->lhs.type);
      }
      break;

    case NodeObject::OR:
      ret += "OR( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        Or* ptr = dynamic_cast<Or*>(current.node);

        std::for_each(ptr->operands.rbegin(),
                      ptr->operands.rend(),
                      [&bfs](NodeObject& nobj) {
                        bfs.emplace(nobj.object.get(), nobj.type);
                      });
      }
      break;

    case NodeObject::AND:
      ret += "AND( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        And* ptr = dynamic_cast<And*>(current.node);

        std::for_each(ptr->operands.rbegin(),
                      ptr->operands.rend(),
                      [&bfs](NodeObject& nobj) {
                        bfs.emplace(nobj.object.get(), nobj.type);
                      });
      }
      break;

    case NodeObject::NOT:
      ret += "NOT( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        Not* ptr = dynamic_cast<Not*>(current.node);

        bfs.emplace(ptr->operand.object.get(), ptr->operand.type);
      }
      break;

    case NodeObject::EQ:
      ret += "EQ( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        Eq* ptr = dynamic_cast<Eq*>(current.node);

        bfs.emplace(ptr->right.object.get(), ptr->right.type);
        bfs.emplace(ptr->left.object.get(), ptr->left.type);
      }
      break;

    case NodeObject::REL:
      ret += "REL( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        Rel* ptr = dynamic_cast<Rel*>(current.node);

        bfs.emplace(ptr->right.object.get(), ptr->right.type);
        bfs.emplace(ptr->left.object.get(), ptr->left.type);
      }
      break;

    case NodeObject::ADD:
      ret += "ADD( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        Add* ptr = dynamic_cast<Add*>(current.node);

        std::for_each(ptr->operands.rbegin(),
                      ptr->operands.rend(),
                      [&bfs](NodeObject& nobj) {
                        bfs.emplace(nobj.object.get(), nobj.type);
                      });
      }
      break;

    case NodeObject::MUL:
      ret += "MUL( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        Mul* ptr = dynamic_cast<Mul*>(current.node);

        std::for_each(ptr->operands.rbegin(),
                      ptr->operands.rend(),
                      [&bfs](NodeObject& nobj) {
                        bfs.emplace(nobj.object.get(), nobj.type);
                      });
      }
      break;

    case NodeObject::LIST:
      ret += "LIST_EXPR( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        List* ptr = dynamic_cast<List*>(current.node);

        std::for_each(ptr->operands.rbegin(),
                      ptr->operands.rend(),
                      [&bfs](NodeObject& nobj) {
                        bfs.emplace(nobj.object.get(), nobj.type);
                      });

        bfs.emplace(ptr->initial.object.get(), ptr->initial.type);
      }
      break;

    case NodeObject::ID:
      ret += "ID[";
      {
        Identifier* ptr = dynamic_cast<Identifier*>(current.node);

        ret += ptr->str;
        ret += "] ";
      }
      break;

    case NodeObject::INT:
      ret += "INT[";
      {
        Integer* ptr = dynamic_cast<Integer*>(current.node);

        ret += std::to_string(ptr->value);
        ret += "] ";
      }
      break;

    case NodeObject::BOOL:
      ret += "BOOL ";
      break;

    case NodeObject::LLIT:
      ret += "LLIT( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        ListLiteral* ptr = dynamic_cast<ListLiteral*>(current.node);

        std::for_each(ptr->exprs.rbegin(),
                      ptr->exprs.rend(),
                      [&bfs](NodeObject& nobj) {
                        bfs.emplace(nobj.object.get(), nobj.type);
                      });
      }
      break;

    case NodeObject::FLIT:
      ret += "FLIT( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        FuncLiteral* ptr = dynamic_cast<FuncLiteral*>(current.node);

        if(ptr->ret.object) {
          bfs.emplace(ptr->ret.object.get(), ptr->ret.type);
        }

        std::for_each(ptr->ops.rbegin(),
                      ptr->ops.rend(),
                      [&bfs](NodeObject& nobj) {
                        bfs.emplace(nobj.object.get(), nobj.type);
                      });
      }
      break;

    case NodeObject::FAPPLY:
      ret += "FAPPLY( ";
      bfs.emplace(nullptr, NodeObject::NONE);
      {
        FuncApply* ptr = dynamic_cast<FuncApply*>(current.node);

        std::for_each(ptr->exprs.rbegin(),
                      ptr->exprs.rend(),
                      [&bfs](NodeObject& nobj) {
                        bfs.emplace(nobj.object.get(), nobj.type);
                      });
      }
      break;

    default:
      ret += ") ";
      break;
    }
  }

  return ret;
}
