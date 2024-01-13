#include <__expected/unexpect.h>
#include <__expected/unexpected.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <expected>

#include "tokens.h"

namespace redml {

  std::expected<std::vector<Token>, std::string> tokenize(std::string line) {
  int idx = 0;
  std::vector<Token> buffer;
  while(idx < line.size()) {

    // Read in a number literal
    int start = idx;
    if(std::isdigit(line[idx])) {
      std::string num;
      bool fl = false;
      while(std::isalnum(line[idx]) || line[idx] == '.') {
        num += line[idx];
        switch(line[idx]) {
          case '.':
            fl = true;
            break;
          case 'f':
            if(std::isalnum(line[idx+1])) {
              return std::unexpected("Error: literal type specifier must be last character of number.");
            }
            fl = true;
            idx++;
            goto push_num;
          default:
            break;
        }
        idx++;
      }

push_num:
      if(fl) {
        buffer.push_back(Token{
            .type = FLOAT_LIT,
            .symbol = num
            });
      } else {
        buffer.push_back(Token{
            .type = INT_LIT,
            .symbol = num
            });
      }
    }

    // Look for keywords and user defined symbols
    if(std::isalpha(line[idx])) {
      while(std::isalnum(line[idx])) {
        idx++;
      }
      std::string tmp(line.begin() + start, line.begin() + idx);
      auto keyword = std::find(TOKENSYMBOLS.begin(), TOKENSYMBOLS.end(), tmp);
      if(keyword == TOKENSYMBOLS.end()) {
        buffer.emplace_back(Token{IDENT, tmp});
      } else {
        buffer.emplace_back(Token{static_cast<TokenType>(std::distance(TOKENSYMBOLS.begin(), keyword)), tmp});
      }
    }

    // If it's not a keyword or an identifier, look in punctuation
    switch(line[idx]) {
      case '+':
        buffer.emplace_back(Token{
            .type = ADD,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[ADD])
            });
        break;
      case '-':
        buffer.emplace_back(Token{
            .type = SUB,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[SUB])
            });
        break;
      case '*':
        buffer.emplace_back(Token{
            .type = MUL,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[MUL])
            });
        break;
      case '/':
        buffer.emplace_back(Token{
            .type = DIV,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[DIV])
            });
        break;
      case '(':
        buffer.emplace_back(Token{
            .type = LEFT_PAREN,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[LEFT_PAREN])
            });
        break;
      case ')':
        buffer.emplace_back(Token{
            .type = RIGHT_PAREN,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[RIGHT_PAREN])
            });
        break;
      case '{':
        buffer.emplace_back(Token{
            .type = LEFT_BRACK,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[LEFT_BRACK])
            });
        break;
      case '}':
        buffer.emplace_back(Token{
            .type = RIGHT_BRACK,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[RIGHT_BRACK])
            });
        break;
      case '[':
        buffer.emplace_back(Token{
            .type = LEFT_SQUARE,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[LEFT_SQUARE])
            });
        break;
      case ']':
        buffer.emplace_back(Token{
            .type = RIGHT_SQUARE,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[RIGHT_SQUARE])
            });
        break;
      case ';':
        buffer.emplace_back(Token{
            .type = SEMI_COL,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[SEMI_COL])
            });
        break;
      case ':':
        buffer.emplace_back(Token{
            .type = COL,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[COL])
            });
        break;
    }

    idx++;
  }
  return buffer;
}

std::vector<uint8_t> compile(std::string line) {

  auto res = tokenize(line);
  if(!res.has_value()) {
    std::cout << res.error() << "\n";
    return {};
  }

  auto tokens = res.value();

  for(auto t: tokens) {
    std::cout << t.symbol << " -> " << t.type << "\n";
  }

  return {};
}

void repl_main() {
  std::string line;
  uint32_t scope = 0;
  std::vector<uint8_t> bytecode;
  while(true) {
    if(line.empty()) {
      std::cout << ":>";
    }
    std::string cur;
    std::getline(std::cin, cur);
    line.append(cur);
    if(line == "q") {
      return;
    }
    auto last_tok = std::find_if(line.rbegin(), line.rend(), [](const auto& c) {
        return !std::isspace(c);
        });

    if(*last_tok == '{' ||
       *last_tok == '(' ||
       *last_tok == '\"') {
      scope++;
      continue;
    } else if(*last_tok == '}' ||
              *last_tok == ')' ||
              *last_tok == '\"') {
      scope--;
      if(scope > 0) {
        continue;
      }
      bytecode = compile(line);
    } else if(*last_tok == ';') {
      if(scope) {
        continue;
      } else {
        bytecode = compile(line);
      }
    } else {
      std::cerr << "Error: Statements must be terminated using \";\".\n";
    }
    line.clear();
  }
}
}

auto main(int argc, char**argv) -> int {
  redml::repl_main();
	return 0;
}
