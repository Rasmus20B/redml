module;

#include <type_traits>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <expected>
#include <vector>

import interpret.token;
export module interpret.parser;

enum class NodeType {
  ROOT,
  FUNC_DECL,
  FUNC_DEF,
  FUNC_CALL,
  ASSIGNMENT,
  IDENT,
  COMPOUND_STATEMENT,
  INT_LIT,
  SIZE,
};

enum class NodeClass {
  UNARY,
  BINARY,
  TERNARY,
  NARY,
  SIZE
};

static std::unordered_map<NodeType, NodeClass> NodeGroups {{
  {NodeType::ROOT, NodeClass::NARY},
  {NodeType::FUNC_DECL, NodeClass::UNARY},
  {NodeType::FUNC_DEF, NodeClass::UNARY},
  {NodeType::FUNC_CALL, NodeClass::BINARY},
  {NodeType::ASSIGNMENT, NodeClass::BINARY},
  {NodeType::IDENT, NodeClass::BINARY},
  {NodeType::COMPOUND_STATEMENT, NodeClass::NARY},
  {NodeType::INT_LIT, NodeClass::BINARY}
}};

static std::unordered_map<TokenType, int> precedence {{
  {IDENT, -1},
  {INT_LIT, -1},
  {SEMI_COL, -1},
  {EQ, -1},
  {ADD, 10},
  {MUL, 11},
  {SUB, 15},
  {DIV, 20}
}};

struct Expr {
  Expr(NodeType t) : type(t) {}
  Expr() = default;
  ~Expr() = default;
  public:
    NodeType type;
};

using e_ptr = std::shared_ptr<Expr>;

struct Unary : public Expr {
  Unary(e_ptr c, NodeType t, std::string_view et, std::string_view val) : Expr(t), child(std::move(c)), etype(et), val(val) {}

  e_ptr child;
  std::string etype;
  std::string val;
};

struct Binary : public Expr {
  Binary(NodeType t, e_ptr c1, e_ptr c2, std::string val) : Expr(t), lchild(std::move(c1)), rchild(std::move(c2)), val(val) {}
  e_ptr lchild;
  e_ptr rchild;
  std::string val;
};

struct Ternary : public Expr {
  e_ptr lchild;
  e_ptr mchild;
  e_ptr rchild;
};

struct Nary : public Expr {
  std::vector<e_ptr> children;
};

export class parser {

  public:

    parser(std::vector<Token> t) : tokens(t) {
      tree = Nary(NodeType::ROOT);
    }

    void print_unary_expr(e_ptr e) {
      auto tmp = static_cast<Unary*>(e.get());
      std::cout << "OP: " << tmp->val << "\n";
      if(NodeGroups[e.get()->type] == NodeClass::UNARY) {
        print_unary_expr(std::move(tmp->child));
      }
    }

    void print_binary_expr(e_ptr e) {

      auto type = NodeGroups[e.get()->type];
      if(type == NodeClass::BINARY) {
        auto tmp = static_cast<Binary*>(e.get());
        std::cout << "BINARY OP: " << (tmp->val) << "\n";
        if(tmp->lchild != nullptr) {
          std::cout << "Going left\n";
          print_binary_expr(tmp->lchild);
        }
        if(tmp->rchild != nullptr) {
          std::cout << "Going right\n";
          print_binary_expr(tmp->rchild);
        }
        if(type == NodeClass::UNARY) {
          print_unary_expr(e);
        }
      }
    }

    void print_compound(e_ptr e) {
      auto ch = static_cast<Nary*>(e.get());

      for(auto &s : ch->children) {
        std::cout << "Type of child expr: " << static_cast<int>(s->type) << "\n";
      }
    }

    void print_tree() {
      for(auto &ci : tree.children) {
        switch(ci.get()->type) {
          case NodeType::ASSIGNMENT:
            std::cout << "FODUN ASIGNEMN\n";
            print_binary_expr(ci);
            break;
          case NodeType::FUNC_DECL:
            std::cout << "Function Delcaration: " << static_cast<Unary*>(ci.get())->val << " -> " << static_cast<Unary*>(ci.get())->etype << "\n";
            print_compound(ci);
            break;
          case NodeType::FUNC_DEF:
            std::cout << "Function Definition: " << static_cast<Unary*>(ci.get())->val << " -> " << static_cast<Unary*>(ci.get())->etype << "\n";
            print_compound(ci);
            break;
          default:
            break;
        }
      }
    }

    void parse_program() {
      while(idx < tokens.size()) {
        Token a = tokens[idx];
        switch (a.type) {
          case TYPE_INT:
            tree.children.push_back((parse_int()));
            break;
          case TYPE_VOID:
            break;
          case SEMI_COL:
            idx++;
            break;
          default:
            goto end_parse;
        }
      }   

end_parse:
      print_tree();
    }

  private:
    e_ptr parse_statement() {
      auto a = next_token();
      switch(a.type) {
        case LEFT_BRACK:
          return parse_compound_statement();
        case INT_LIT:
          break;
        case TYPE_INT:
          return parse_int();
        case IDENT:
          break;
        case RETURN:
          break;
        case RIGHT_BRACK:
          return nullptr;
        default:
          return nullptr;
      }
      return nullptr;
    }

    e_ptr parse_compound_statement() {
      auto cs = Nary(NodeType::COMPOUND_STATEMENT);

      while(tokens[idx].type != RIGHT_BRACK) {
        cs.children.push_back(parse_statement());
      }
      idx++;
      return std::make_unique<Nary>(std::move(cs));
    }

    e_ptr parse_func_decl(std::string_view ident, std::string_view ftype) {
      while(tokens[idx].type != RIGHT_PAREN) {
        idx++;
      }
      idx++;

      switch(tokens[idx].type) {
        case LEFT_BRACK:
          return std::make_unique<Unary>(
              parse_compound_statement(),
              NodeType::FUNC_DEF,
              ftype,
              ident
              );
          break;
        case SEMI_COL:
          return std::make_unique<Unary>(
              nullptr,
              NodeType::FUNC_DECL,
              ftype,
              ident
              );
          break;
        default:
          return nullptr;
      }
    }

    e_ptr parse_f_args() {
      idx++;
      auto t = tokens[idx];
      if (t.type == COMMA) {
        t = next_token();
      }
    }

    e_ptr parse_identifier_u() {
      if(tokens[idx+1].type == LEFT_PAREN) {
        idx++;
        return std::make_unique<Binary>(
          NodeType::FUNC_CALL,
          nullptr,
          parse_f_args(),
          tokens[idx].symbol
        );
      } else {
        std::cout << "FOUND IDENT: " << tokens[idx].symbol << "\n";
        auto num = std::make_unique<Binary>(
            NodeType::ASSIGNMENT,
            nullptr,
            nullptr,
            tokens[idx].symbol
            );
        num->val = tokens[idx].symbol;
        idx++;
        return num;
      }
    }

    e_ptr parse_int_literal_u() {
      std::cout << "FOUND A LIT: " << tokens[idx].symbol << "\n";
      auto num = std::make_unique<Binary>(
          NodeType::INT_LIT,
          nullptr,
          nullptr,
          tokens[idx].symbol
          );
      idx++;
      return num;
    }

    e_ptr parse_primary_expr() {
      switch(tokens[idx].type) {
        case IDENT:
          return parse_identifier_u();
        case INT_LIT:
          return parse_int_literal_u();
        case LEFT_PAREN:
          // return parse_paren_expr_u();
        default:
          return nullptr;
      }
    }

    e_ptr parse_binop_expr(int prec, e_ptr lhs) {
      std::cout << "BINOP STEP TYPE = " << static_cast<int>(lhs->type) << "\n";
      while(true) {
        auto t1 = tokens[idx].type;
        auto v1 = tokens[idx].symbol;
        auto p1 = precedence[tokens[idx].type];
        std::cout << tokens[idx].symbol << "\n";
        if(p1 < prec) {
          return lhs;
        }

        idx++;

        auto rhs = parse_primary_expr();

        if(!rhs) {
          return nullptr;
        }

        auto p2 = precedence[tokens[idx].type];
        if(p1 < p2) {
          rhs = parse_binop_expr(p1 + 1, std::move(rhs));
          if(!rhs) {
            return nullptr;
          }
        }
        lhs = std::make_unique<Binary>(
            NodeType::SIZE,
            (rhs),
            (lhs),
            v1
            );
      }
    }

    e_ptr parse_var_assignment(std::string_view vname, std::string_view vtype) {
      idx--;
      e_ptr lhs = parse_primary_expr();
      // std::cout << static_cast<int>(lhs->type) << "\n";
      next_token();

      return std::make_unique<Binary>(
          NodeType::ASSIGNMENT,
          std::move(lhs),
          parse_binop_expr(0, parse_primary_expr()),
          "="
          );
    }

    e_ptr parse_identifier(std::string_view fname, std::string_view ftype) {
      auto a = next_token();
      switch(a.type) {
        case SEMI_COL:
          return nullptr;
        case LEFT_PAREN:
          if(fname.size() && ftype.size()) {
            return parse_func_decl(fname, ftype);
          }
          break;
        case ASSIGN:
          std::cout << "FOUND VAR ASSIGNMENT\n";
          return parse_var_assignment(fname, ftype);
        default:
          break;
      }
      return nullptr;
    }

    e_ptr parse_int() {
      auto a = next_token();
      std::cout << "Looking @" << a.type << " -> " << a.symbol << "\n";
      switch (a.type) {
        case IDENT:
          return parse_identifier(a.symbol, "int");
        default:
          std::cerr << "Error: " << a.symbol << " is not a valid identifier.\n";
          return nullptr;
      }
    }
    Token next_token() {
      if (idx < tokens.size()) {
        idx += 1;
      }
      return tokens[idx];
    }

    std::vector<Token> tokens;
    int idx{};
    std::string buffer;
    Nary tree;
};
