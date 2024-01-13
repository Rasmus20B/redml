#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "tokenizer.h"

namespace redml {
  std::vector<uint8_t> compile(std::string line);
  void repl_main();
}
