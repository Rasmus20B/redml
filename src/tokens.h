#pragma once

#include <string>
#include <string_view>
#include <array>

namespace redml {


  enum TokenType  {
    TYPE_INT,
    TYPE_VOID,
    IDENT,
    INT_LIT,
    FLOAT_LIT,
    ADD,
    SUB,
    MUL,
    DIV,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACK,
    RIGHT_BRACK,
    LEFT_SQUARE,
    RIGHT_SQUARE,
    SEMI_COL,
    COL,
    RETURN,
    SIZE
  };

  static constexpr std::array<std::string_view, TokenType::SIZE> TOKENSYMBOLS {{
    "int",
    "void",
    "",
    "",
    "",
    "+",
    "-",
    "*",
    "/",
    "(",
    ")",
    "{",
    "}",
    "[",
    "]",
    ";",
    ":",
    "return"
  }};

  struct Token {
    TokenType type;
    std::string symbol;
  };
}
