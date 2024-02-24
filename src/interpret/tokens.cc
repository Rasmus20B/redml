module;

#include <utility>
#include <string>
#include <string_view>
#include <array>

export module interpret.token;

export enum TokenType  {
  TYPE_INT,
  TYPE_VOID,
  IDENT,
  INT_LIT,
  FLOAT_LIT,
  ADD,
  SUB,
  MUL,
  DIV,
  ASSIGN,
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACK,
  RIGHT_BRACK,
  LEFT_SQUARE,
  RIGHT_SQUARE,
  SEMI_COL,
  COL,
  RETURN,
  COMMA,
  EQ,
  IF,
  ELSE,
  SIZE
};

export constexpr std::array<std::string_view, std::to_underlying(TokenType::SIZE)> TOKENSYMBOLS {{
  "int",
  "void",
  "",
  "",
  "",
  "+",
  "-",
  "*",
  "/",
  "=",
  "(",
  ")",
  "{",
  "}",
  "[",
  "]",
  ";",
  ":",
  "return",
  ",",
  "==",
  "if",
  "else",
}};

export struct Token {
  TokenType type;
  std::string symbol;
};
