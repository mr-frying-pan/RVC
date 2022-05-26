#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <memory>
#include <optional>
#include <string>
#include <cstdint>

#include "Parser.hpp"

class SyntaxError {
public:
  virtual std::string to_string() const noexcept = 0;
};

class ArgCountError : public SyntaxError {
private:
  int expected;
  int got;
public:
  ArgCountError(int expected, int got) noexcept;
  std::string to_string() const noexcept override;
};


/**
 * Error to be used when other kind of operand should have been provided (e.g. register instead of immediate).
 */
class OperandTypeError : public SyntaxError {
private:
  std::string expected; /**< expected operand type */
public:
  OperandTypeError(const std::string& expected) noexcept;
  std::string to_string() const noexcept override;
};


/**
 * Error signifying that low bits of an immediate would be truncated if it were compiled.
 */
class ImmediateWillBeTruncatedError : public SyntaxError {
private:
  std::uint32_t imm; /**< value used as an operand */
  std::uint8_t lowBit; /**< highest bit of imm which would be lost */
public:
  ImmediateWillBeTruncatedError(std::uint32_t imm, std::uint8_t lowBit) noexcept;
  std::string to_string() const noexcept override;
};


/**
 * Error signifying that high bits of an immediate would be dropped if it were compiled.
 */
class ImmediateTooBigError : public SyntaxError {
private:
  std::uint32_t imm; /**< value used as an operand */
  std::uint8_t highBit; /**< lowest bit which woud be lost */
public:
  ImmediateTooBigError(std::uint32_t imm, std::uint8_t highBit) noexcept;
  std::string to_string() const noexcept override;
};


/**
 * Error signifying that instruction is not among known instructions.
 */
class UnrecognizedInstructionError : public SyntaxError {
private:
  std::string instr;
public:
  UnrecognizedInstructionError(const std::string& instr) noexcept;
  std::string to_string() const noexcept override;
};


std::unique_ptr<SyntaxError> check(const ParseNode &node) noexcept;

#endif
