#include <catch2/catch.hpp>
#include <string>
#include "../Parser.hpp"
#include "../Compiler.hpp"
#include "../constants.hpp"

#define COMP_I(INSTR, OPCODE, RD, FUNCT3, RS1, IMM, FULL)		\
  TEST_CASE(INSTR, "[compiler][" INSTR "]") {				\
    std::string line = INSTR " x" #RD " x" #RS1 " " #IMM;		\
    auto parsed = parse(line);						\
    auto compiled = compile(parsed);					\
    Instruction instr = compiled.first;					\
    std::string label = compiled.second;				\
    REQUIRE(instr.getOpcode() == OPCODE);				\
    REQUIRE(instr.getRd() == RD);					\
    REQUIRE(instr.getFunct3() == FUNCT3);				\
    REQUIRE(instr.getRs1() == RS1);					\
    REQUIRE(instr.getImmI() == IMM);					\
    REQUIRE(instr.value.to_ulong() == FULL);				\
  }

COMP_I("ADDI", constants::opcode::OP_IMM, 1, constants::funct3::ADDI, 2, 20,
       0b00000001010000010000000010010011);
COMP_I("SLTI", constants::opcode::OP_IMM, 1, constants::funct3::SLTI, 2, 20,
       0b00000001010000010010000010010011);
COMP_I("SLTIU", constants::opcode::OP_IMM, 1, constants::funct3::SLTIU, 2, 20,
       0b00000001010000010011000010010011);
COMP_I("ANDI", constants::opcode::OP_IMM, 1, constants::funct3::ANDI, 2, 20,
       0b00000001010000010111000010010011);
COMP_I("ORI", constants::opcode::OP_IMM, 1, constants::funct3::ORI, 2, 20,
       0b00000001010000010110000010010011);
COMP_I("XORI", constants::opcode::OP_IMM, 1, constants::funct3::XORI, 2, 20,
       0b00000001010000010100000010010011);
