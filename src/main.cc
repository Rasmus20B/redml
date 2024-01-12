#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> tokenize(std::string line) {
  int idx = 0;
  std::vector<std::string> buffer;
  while(idx < line.size()) {
    if(std::isalpha(line[idx])) {
      int start = idx;
      while(std::isalnum(line[idx])) {
        idx++;
      }
      std::string tmp(line.begin() + start, line.begin() + idx);
      buffer.emplace_back(tmp);
    }
    idx++;
  }
  return buffer;
}

std::vector<uint8_t> compile(std::string line) {

  std::vector<std::string> tokens;
  tokens = tokenize(line);

  for(auto i: tokens) {
    std::cout << i << "\n";
  }
  return {};
}

void repl_main() {
  std::string line;
  while(true) {
    std::string cur;
    std::getline(std::cin, cur);
    line.append(cur);
    if(line == "q") {
      return;
    }
    auto last_tok = std::find_if(line.rbegin(), line.rend(), [](const auto& c) {
        return !std::isspace(c);
        });

    if(*last_tok != ';') {
      line.append(" ");
      continue;
    }
    std::cout << "Final Line before compilation: " << line << "\n";
    auto bytecode = compile(line);
    line.clear();
  }
}

auto main(int argc, char**argv) -> int {
  repl_main();
	return 0;
}
