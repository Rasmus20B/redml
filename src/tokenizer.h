#pragma once

#include <expected>
#include <vector>
#include <string>
#include "tokens.h"

namespace redml {
  std::expected<std::vector<Token>, std::string> tokenize(std::string line);
}
