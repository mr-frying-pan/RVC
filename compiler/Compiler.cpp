#include "Compiler.hpp"

#include <unordered_map>

#include "constants.hpp"

std::pair<Instruction, std::string> compileI(const uint32_t opcode, const uint32_t funct3, const ParseNode& node) {
  uint8_t rd = std::get<RegisterOperand>(node.operands[0]).value();
  uint8_t rs1 = std::get<RegisterOperand>(node.operands[1]).value();
  uint32_t imm = std::get<ImmediateOperand>(node.operands[2]).value();
  return { Instruction { opcode, funct3, rd, rs1, imm }, "" };
}

typedef std::pair<Instruction, std::string>(*cplF)(const ParseNode&);

#define MAP_LAMBDA(INSTR, COMPL)		\
  {INSTR,  [] (const ParseNode& node) {		\
    return COMPL;				\
  }}

static const std::unordered_map<std::string, cplF> compileFs = {
  MAP_LAMBDA("ADDI", compileI(constants::opcode::OP_IMM, constants::funct3::ADDI, node)),
  MAP_LAMBDA("SLTI", compileI(constants::opcode::OP_IMM, constants::funct3::SLTI, node)),
  MAP_LAMBDA("SLTIU", compileI(constants::opcode::OP_IMM, constants::funct3::SLTIU, node)),
  MAP_LAMBDA("ANDI", compileI(constants::opcode::OP_IMM, constants::funct3::ANDI, node)),
  MAP_LAMBDA("ORI", compileI(constants::opcode::OP_IMM, constants::funct3::ORI, node)),
  MAP_LAMBDA("XORI", compileI(constants::opcode::OP_IMM, constants::funct3::XORI, node)),
};

std::pair<Instruction, std::string> compile(const ParseNode& node) {
  if (!node.label)
    return compileFs.at(node.instr)(node);
  return { Instruction::zero, node.instr };
}
