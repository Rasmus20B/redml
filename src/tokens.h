#pragma once

#include <string>

namespace redml {

enum class TokenType {
  TYPE_INT,
  TYPE_VOID,
  IDENT,
  ADD,
  SUB,
  MUL,
  DIV,
  RETURN,
};

struct Token {
  TokenType type;
  std::string symbol;
};
}
