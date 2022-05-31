#include "Checker.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <iostream>
#include <variant>

#include "Parser.hpp"
#include "constants.hpp"

#include <bitset>


ArgCountError::ArgCountError(int expected, int got) noexcept
  : expected(expected), got(got) {}

std::string ArgCountError::to_string() const noexcept {
  return "Expected " + std::to_string(expected) + " arguments, got " + std::to_string(got) + ".";
}


OperandTypeError::OperandTypeError(const std::string &expected) noexcept
    : expected(expected) {}

std::string OperandTypeError::to_string() const noexcept {
  return "Expected operand to be a " + expected + ".";
}


LowBitsLostError::LowBitsLostError(std::uint32_t imm, std::uint8_t lowBit) noexcept
  : imm(imm), lowBit(lowBit) {}

std::string LowBitsLostError::to_string() const noexcept {
  return std::to_string(lowBit) + " lower bits would be lost in value " + std::to_string(imm) + ".";
}


HighBitsLostError::HighBitsLostError(std::uint32_t imm,std::uint8_t highBit) noexcept
  : imm(imm), highBit(highBit) {}

std::string HighBitsLostError::to_string() const noexcept {
  return std::to_string(highBit) + " high bits would be lost in value " + std::to_string(imm) + ".";
}


UnrecognizedInstructionError::UnrecognizedInstructionError(const std::string &instr) noexcept
    : instr(instr) {}

std::string UnrecognizedInstructionError::to_string() const noexcept {
  return "Unrecognised instruction " + instr + ".";
}

static std::unique_ptr<SyntaxError>
isRegisterOperand(const std::variant<RegisterOperand, ImmediateOperand, LabelOperand>& operand) {
  if (!std::holds_alternative<RegisterOperand>(operand))
    return std::unique_ptr<SyntaxError>(new OperandTypeError("register"));
  return nullptr;
}

static std::unique_ptr<SyntaxError>
isImmediateOperand(const std::variant<RegisterOperand, ImmediateOperand, LabelOperand>& operand,
		   const std::uint8_t minImmBit,
		   const std::uint8_t maxImmBit) {
  if (!std::holds_alternative<ImmediateOperand>(operand))
    return std::unique_ptr<SyntaxError>(new OperandTypeError("immediate"));

  uint32_t value = std::get<ImmediateOperand>(operand).value();

  std::bitset<32> bitv = value;
  std::bitset<32> lowmask    = constants::bitmasks::lowBits(minImmBit);
  std::bitset<32> highmask   = constants::bitmasks::highBits(32 - maxImmBit - 1);
  std::bitset<32> lowmasked  = value & constants::bitmasks::lowBits(minImmBit);
  std::bitset<32> highmasked = value & constants::bitmasks::highBits(32 - maxImmBit - 1);

  std::cout << value << ":" << std::endl
	    << "\t         v: " << bitv << std::endl
	    << "\t   lowmask: " << lowmask << " (" << (int)minImmBit << ")" << std::endl
	    << "\t  highmask: " << highmask << " (" << (int)maxImmBit << ")" << std::endl
	    << "\t lowmasked: " << lowmasked << " (" << lowmasked.any() << ")" << std::endl
	    << "\thighmasked: " << highmasked << " (" << highmasked.any() << ")" << std::endl;

  if (value & constants::bitmasks::lowBits(minImmBit)) // true when low bits are non-zero
    return std::unique_ptr<SyntaxError>(new LowBitsLostError(value, minImmBit));

  if (value & constants::bitmasks::highBits(32 - maxImmBit - 1)) // true when high bits are non-zero
    return std::unique_ptr<SyntaxError>(new HighBitsLostError(value, 32 - maxImmBit));
  
  return nullptr;
}

static std::unique_ptr<SyntaxError>
isLabelOperand(const std::variant<RegisterOperand, ImmediateOperand, LabelOperand>& operand) {
  if (!std::holds_alternative<LabelOperand>(operand))
    return std::unique_ptr<SyntaxError>(new OperandTypeError("label"));
  return nullptr;
}

static std::unique_ptr<SyntaxError> checkInstr(const ParseNode& node, const std::string& argFormat,
					       const uint8_t minImmBit, const uint8_t maxImmBit) {
  if (node.operands.size() != argFormat.length())
    return std::unique_ptr<SyntaxError>(new ArgCountError(argFormat.length(), node.operands.size()));

  // for each required parameter type check passed parameter
  for(size_t i = 0; i < argFormat.length(); i++) {
    switch (argFormat[i]) {
    case 'r': {
      auto check_result = isRegisterOperand(node.operands[i]);
      if (check_result.get() != nullptr)
	return check_result;
      break;
    }
    case 'i': {
      auto check_result = isImmediateOperand(node.operands[i], minImmBit, maxImmBit);
      if (check_result.get() != nullptr)
	return check_result;
      break;
    }
    case 't': {
      if (std::holds_alternative<ImmediateOperand>(node.operands[i]))
	return isImmediateOperand(node.operands[i], minImmBit, maxImmBit);
      // else fall through to l case
    }
    case 'l':
      return isLabelOperand(node.operands[i]);
    }
  }
  return nullptr;
}

/**
 * Helper macro to define instruction map.
 * @param key        instruction name (as it is supposed to be typed in the file
 * @param argFormat  a string, one character per parameter required:
 *                       r – register
 *                       i – immediate
 *                       t – either label or immediate
 * @param minVal     lowest bit of an immediate which will be included in the resulting machine code
 * @param maxVal     highest bit on an immediate which will be included in the resulting machine code
 */
#define MAP_LAMBDA(key, argFormat, minVal, maxVal)      \
  {key, [] (const ParseNode& node) {                    \
    return checkInstr(node, argFormat, minVal, maxVal); \
  }}

// Shortcut typedef
typedef std::unique_ptr<SyntaxError> (*chF)(const ParseNode &);

// Map of instructions to checking function with correct parameters for that instruction
static const std::unordered_map<std::string, chF> checkFs = {
  MAP_LAMBDA("ADDI", "rri", 0, 11),
  MAP_LAMBDA("SLTI", "rri", 0, 11),
  MAP_LAMBDA("SLTIU", "rri", 0, 11),
  MAP_LAMBDA("ANDI", "rri", 0, 11),
  MAP_LAMBDA("ORI", "rri", 0, 11),
  MAP_LAMBDA("XORI", "rri", 0, 11),
  MAP_LAMBDA("SLLI", "rri", 0, 4),
  MAP_LAMBDA("SRLI", "rri", 0, 4),
  MAP_LAMBDA("SRAI", "rri", 0, 4),
  MAP_LAMBDA("LUI", "rt", 12, 31),
  MAP_LAMBDA("AUIPC", "rt", 12, 31),
  MAP_LAMBDA("ADD", "rrr", 0, 0),
  MAP_LAMBDA("SLT", "rrr", 0, 0),
  MAP_LAMBDA("SLTU", "rrr", 0, 0),
  MAP_LAMBDA("AND", "rrr", 0, 0),
  MAP_LAMBDA("OR", "rrr", 0, 0),
  MAP_LAMBDA("XOR", "rrr", 0, 0),
  MAP_LAMBDA("SLL", "rrr", 0, 0),
  MAP_LAMBDA("SRL", "rrr", 0, 0),
  MAP_LAMBDA("SUB", "rrr", 0, 0),
  MAP_LAMBDA("SRA", "rrr", 0, 0),
  MAP_LAMBDA("JAL", "rl", 0, 0),
  MAP_LAMBDA("JALR", "rri", 0, 11),
  MAP_LAMBDA("BEQ", "rrl", 0, 0),
  MAP_LAMBDA("BNE", "rrl", 0, 0),
  MAP_LAMBDA("BLT", "rrl", 0, 0),
  MAP_LAMBDA("BLTU", "rrl", 0, 0),
  MAP_LAMBDA("BGE", "rrl", 0, 0),
  MAP_LAMBDA("BGEU", "rrl", 0, 0),
  MAP_LAMBDA("LW", "rrl", 0, 0),
  MAP_LAMBDA("LW", "rrl", 0, 0),
  MAP_LAMBDA("LH", "rrl", 0, 0),
  MAP_LAMBDA("LHU", "rrl", 0, 0),
  MAP_LAMBDA("LB", "rrl", 0, 0),
  MAP_LAMBDA("LBU", "rrl", 0, 0),
  MAP_LAMBDA("SW", "rrl", 0, 0),
  MAP_LAMBDA("SH", "rrl", 0, 0),
  MAP_LAMBDA("SB", "rrl", 0, 0),
  MAP_LAMBDA("ECALL", "", 0, 0),
  MAP_LAMBDA("EBREAK", "", 0, 0)
};

// Remove macro after map was defined.
#undef MAP_LAMBDA

std::unique_ptr<SyntaxError> check(const ParseNode &node) noexcept {
  try {
    if (!node.label)
      return checkFs.at(node.instr)(node);
    return nullptr;
  }
  catch(std::out_of_range& e) {
    return std::unique_ptr<SyntaxError>(new UnrecognizedInstructionError(node.instr));
  }
}
