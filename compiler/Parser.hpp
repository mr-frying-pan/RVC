#ifndef PARSER_HPP
#define PARSER_HPP

#include <optional>
#include <variant>
#include <vector>

#include "Instruction.hpp"

class RegisterOperand {
private:
  uint8_t val;
  RegisterOperand(const uint8_t val) noexcept;
public:
  uint8_t value() const noexcept;
  static std::optional<RegisterOperand> tryConstruct(const std::string& str) noexcept;
};

class ImmediateOperand {
private:
  uint32_t val;
  ImmediateOperand(const uint32_t val) noexcept;
public:
  uint32_t value() const noexcept;
  static std::optional<ImmediateOperand> tryConstruct(const std::string& str) noexcept;
};

class LabelOperand {
private:
  std::string val;
public:
  std::string value() const noexcept;
  LabelOperand(const std::string &lbl) noexcept;
};

struct ParseNode {
  std::string instr;
  std::vector<std::variant<RegisterOperand, ImmediateOperand, LabelOperand>> operands;
  bool label;
};

ParseNode parse(const std::string &line) noexcept;

#endif
