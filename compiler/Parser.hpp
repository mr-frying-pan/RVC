#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

/**
 * Class to store information about register operand in an assembly command.
 *
 * Keeps the register number.
 */
class RegisterOperand {
private:
  std::uint8_t val;
  RegisterOperand(const uint8_t val) noexcept;
public:
  std::uint8_t value() const noexcept;

  /**
   * Method to try and create an instance of this class. Performs syntax checking.
   *
   * @param str    string which should contain register name.
   * @returns      either class instance or std::nullopt if there was a syntax error.
   */
  static std::optional<RegisterOperand> tryConstruct(const std::string& str) noexcept;
};


/**
 * Class to store information about immediate operand in an assembly command.
 *
 * Keeps the immediate value.
 */
class ImmediateOperand {
private:
  std::uint32_t val;
  ImmediateOperand(const uint32_t val) noexcept;
public:
  std::uint32_t value() const noexcept;

  /**
   * Method to try and create an instance of this class. Performs syntax checking.
   *
   * @param str    string which should contain a number (for now decimal only).
   * @returns      either class instance or std::nullopt if {@link str} does not contain a number.
   */
  static std::optional<ImmediateOperand> tryConstruct(const std::string& str) noexcept;
};


/**
 * Class to store information about label operand in an assembly command.
 *
 * Keeps the label name.
 */
class LabelOperand {
private:
  std::string val;
public:
  std::string value() const noexcept;
  LabelOperand(const std::string &lbl) noexcept;
};


/**
 * Struct to hold the parsed instruction.
 */
struct ParseNode {
  std::string instr; /**< instruction name */
  std::vector<std::variant<RegisterOperand, ImmediateOperand, LabelOperand>> operands; /**< operands */
  bool label; /**< flag if this is a label in code */
};

/**
 * Main parsing function.
 *
 * From a line of code creates a {@link ParseNode} containing operand objects. Operands are either registers,
 * immediates or labels, in this order: first checks if operand is a register, then if it is an immediate
 * and if both checks fail then declares it a label. Label existence checks are not performed here.
 *
 * @param line    line of code
 * @returns       parsed code line
 */
ParseNode parse(const std::string &line) noexcept;

#endif
