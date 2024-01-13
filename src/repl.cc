#include "repl.h"

namespace redml {
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

    if(line == "quit") {
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
