module;

#include <utility>
#include <expected>
#include <vector>

import interpret.token;

export module interpret.tokenizer;

export std::expected<std::vector<Token>, std::string> tokenize(std::string line) {
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
      case '=':
        buffer.emplace_back(Token{
            .type = ASSIGN,
            .symbol = static_cast<std::string>(TOKENSYMBOLS[ASSIGN])
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
