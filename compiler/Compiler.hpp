#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <string>
#include <tuple>

#include "Instruction.hpp"
#include "Parser.hpp"

std::tuple<Instruction, std::string> compile(const ParseNode& node);

#endif
