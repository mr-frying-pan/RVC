#include "Checker.hpp"

#include <stdexcept>
#include <unordered_map>

static std::string errline(const std::string& expected, const std::string& got) {
  return "Expected " + expected + ", got " + got + ".";
}

static std::string errline(const uint32_t expected, const uint32_t got) {
  return errline(std::to_string(expected) + " arguments", std::to_string(got));
}

static std::optional<std::string> checkInstr(const ParseNode& node, const std::string& argFormat,
					     const uint8_t minImmBit = 0, const uint8_t maxImmBit = 0) {
  if (node.operands.size() != argFormat.length())
    return errline(argFormat.length(), node.operands.size());
  for(size_t i = 0; i < node.operands.size(); i++) {
    if (argFormat[i] == 'r' && !std::holds_alternative<RegisterOperand>(node.operands[i]))
      return "Expected a register as " + std::to_string(i+1) + " argument.";
    if (argFormat[i] == 'i' && !std::holds_alternative<ImmediateOperand>(node.operands[i]))
      return "Expected an immediate as " + std::to_string(i+1) + " argument.";
    if (argFormat[i] == 'i') {
      uint32_t operand = std::get<ImmediateOperand>(node.operands[i]).value();
      if ( !(operand & (~0 >> (32 - minImmBit))) )
	return "Value " + std::to_string(operand) + " is too small. "
	  + std::to_string(minImmBit) + " lower bits would be lost.";
      if( operand >> (maxImmBit + 1) )
	  return "Value " + std::to_string(operand) + " is too big. Max width is " + std::to_string(maxImmBit - minImmBit + 1) + " bits.";
    }
    if (argFormat[i] == 'l' && !std::holds_alternative<LabelOperand>(node.operands[i]))
      return "Expected a label as " + std::to_string(i) + " argument.";
  }
  return std::nullopt;
}

typedef std::optional<std::string>(*chF)(const ParseNode&);
static const std::unordered_map<std::string, chF> checkFs = {
  {"ADDI",  [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 11);
  }},
  {"SLTI",  [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 11);
  }},
  {"SLTIU", [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 11);
  }},
  {"ANDI", [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 11);
  }},
  {"ORI", [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 11);
  }},
  {"XORI", [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 11);
  }},
  {"SLLI", [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 4);
    }},
  {"SRLI", [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 4);
    }},
  {"SRAI", [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 4);
    }},
  {"LUI", [] (const ParseNode& node) {
    return checkInstr(node, "r[il]", 12, 31);
  }},
  {"AUIPC", [] (const ParseNode& node) {
    return checkInstr(node, "r[il]", 12, 31);
  }},
  {"ADD", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"SLT", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"SLTU", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"AND", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"OR", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"XOR", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"SLL", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"SRL", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"SUB", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"SRA", [] (const ParseNode& node) {
    return checkInstr(node, "rrr");
  }},
  {"JAL", [] (const ParseNode& node) {
    return checkInstr(node, "rl");
  }},
  {"JALR", [] (const ParseNode& node) {
    return checkInstr(node, "rri", 0, 11);
  }},
  {"BEQ", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"BNE", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"BLT", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"BLTU", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"BGE", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"BGEU", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"LW", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"LW", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"LH", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"LHU", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"LB", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"LBU", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"SW", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"SH", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"SB", [] (const ParseNode& node) {
    return checkInstr(node, "rrl");
  }},
  {"ECALL", [] (const ParseNode& node) {
    return checkInstr(node, "");
  }},
  {"EBREAK", [] (const ParseNode& node) {
    return checkInstr(node, "");
  }},
};

std::optional<std::string> check(const ParseNode &node) noexcept {
  try {
    if (!node.label)
      return checkFs.at(node.instr)(node);
    return std::nullopt;
  }
  catch(std::out_of_range& e) {
    return "Unrecognized instruction: " + node.instr;
  }
}
