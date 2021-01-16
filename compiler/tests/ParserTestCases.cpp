#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "Parser.hpp"

TEST_CASE("Instr l l l", "[parser]") {
  auto parsed = parse("INSTR a b c");
  REQUIRE(parsed.instr == "INSTR");
  REQUIRE(parsed.operands.size() == 3);
  REQUIRE(std::holds_alternative<LabelOperand>(parsed.operands[0]));
  REQUIRE(std::holds_alternative<LabelOperand>(parsed.operands[1]));
  REQUIRE(std::holds_alternative<LabelOperand>(parsed.operands[2]));
  REQUIRE(parsed.label == false);
}

TEST_CASE("Instr r r i", "[parser]") {
  SECTION("correct registers") {
    auto parsed = parse("INSTR x1 x2 15");
    REQUIRE(parsed.instr == "INSTR");
    REQUIRE(parsed.operands.size() == 3);
    REQUIRE(std::holds_alternative<RegisterOperand>(parsed.operands[0]));
    REQUIRE(std::get<RegisterOperand>(parsed.operands[0]).value() == 1);
    REQUIRE(std::holds_alternative<RegisterOperand>(parsed.operands[1]));
    REQUIRE(std::get<RegisterOperand>(parsed.operands[1]).value() == 2);
    REQUIRE(std::holds_alternative<ImmediateOperand>(parsed.operands[2]));
    REQUIRE(std::get<ImmediateOperand>(parsed.operands[2]).value() == 15);
    REQUIRE(parsed.label == false);
  }

  SECTION("incorrect registers") {
    auto parsed = parse("INSTR x-1 x-2 -15");
    REQUIRE(parsed.instr == "INSTR");
    REQUIRE(parsed.operands.size() == 3);
    REQUIRE(std::holds_alternative<LabelOperand>(parsed.operands[0]));
    REQUIRE(std::get<LabelOperand>(parsed.operands[0]).value() == "x-1");
    REQUIRE(std::holds_alternative<LabelOperand>(parsed.operands[1]));
    REQUIRE(std::get<LabelOperand>(parsed.operands[1]).value() == "x-2");
    REQUIRE(std::holds_alternative<ImmediateOperand>(parsed.operands[2]));
    REQUIRE(std::get<ImmediateOperand>(parsed.operands[2]).value() == static_cast<uint32_t>(-15));
    REQUIRE(parsed.label == false);
  }
}
