#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../Parser.hpp"

TEST_CASE("Instr l l l", "[parser]") {
  auto parsed = parse("INSTR a b c");
  REQUIRE(parsed.instr == "INSTR");
  REQUIRE(parsed.operands.size() == 3);
  REQUIRE(std::holds_alternative<LabelOperand>(parsed.operands[0]));
  REQUIRE(std::holds_alternative<LabelOperand>(parsed.operands[1]));
  REQUIRE(std::holds_alternative<LabelOperand>(parsed.operands[2]));
  REQUIRE(parsed.label == false);
}
