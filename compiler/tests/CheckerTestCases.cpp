#include <catch2/catch.hpp>
#include "Parser.hpp"
#include "Checker.hpp"

// I hope to refactor test case names some day

TEST_CASE("I type (ADDI x1 x2 12)", "[checker][I]") {
  std::string line { "ADDI x1 x2 12" };
  auto parsed = parse(line);
  auto checkResult = check(parsed);
  REQUIRE_FALSE(checkResult.has_value());
}

TEST_CASE("I type (ADDI xr x1 12)", "[checker][I]") {
  std::string line { "ADDI xr x1 12" };
  auto parsed = parse(line);
  auto res = check(parsed);
  REQUIRE(res.has_value());
}

TEST_CASE("I type (ADDI x1 x2 asd)", "[checker][I]") {
  std::string line { "ADDI x1 x2 asd" };
  auto parsed = parse(line);
  auto res = check(parsed);
  REQUIRE(res.has_value());
}
