#ifndef PARSER_HPP
#define PARSER_HPP

#include <variant>

#include "Instruction.hpp"

struct ParseResult {
  Instruction instr;
  std::string label;
};

std::variant<ParseResult, std::string> parse(const std::string line);

#endif
