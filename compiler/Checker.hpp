#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <optional>
#include <string>

#include "Parser.hpp"

std::optional<std::string> check(const ParseNode &node) noexcept;

#endif
