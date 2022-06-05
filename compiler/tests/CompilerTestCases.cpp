#include <catch2/catch.hpp>
#include "../Parser.hpp"
#include "../Compiler.hpp"
#include "../constants.hpp"

TEST_CASE("ADDI", "[compiler]") {
  std::string line = "ADDI x1 x2 20";
  auto parsed = parse(line);
  auto compiled = compile(parsed);
  Instruction instr = compiled.first;
  std::string label = compiled.second;
  REQUIRE(instr.getOpcode() == constants::opcode::OP_IMM);
  REQUIRE(instr.getRd() == 1);
  REQUIRE(instr.getFunct3() == constants::funct3::ADDI);
  REQUIRE(instr.getRs1() == 2);
  REQUIRE(instr.getImmI() == 20);
  REQUIRE(instr.value.to_ulong() == 0b00000001010000010000000010010011);
}
