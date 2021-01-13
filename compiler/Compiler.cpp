#include "Compiler.hpp"

#include <unordered_map>

#include "constants.hpp"

std::tuple<Instruction, std::string> compileI(const uint32_t opcode, const uint32_t funct3, const ParseNode& node) {
  uint8_t rd = std::get<RegisterOperand>(node.operands[0]).value();
  uint8_t rs1 = std::get<RegisterOperand>(node.operands[1]).value();
  uint32_t imm = std::get<ImmediateOperand>(node.operands[2]).value();
  return { Instruction { opcode, funct3, rd, rs1, imm }, "" };
}

typedef std::tuple<Instruction, std::string>(*cplF)(const ParseNode&);
static const std::unordered_map<std::string, cplF> compileFs = {
  {"ADDI",  [] (const ParseNode& node) {
      return compileI(constants::opcode::OP_IMM, constants::funct3::ADDI, node);
  }},
};

std::tuple<Instruction, std::string> compile(const ParseNode& node) {
  if (!node.label)
    return compileFs.at(node.instr)(node);
  return { Instruction::zero, node.instr };
}
