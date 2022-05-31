#include <catch2/catch.hpp>
#include "../Parser.hpp"
#include "../Checker.hpp"

#define CHECKER_SUCCESS(CATEGORY, LINE)					\
  TEST_CASE("Successful " CATEGORY " instruction (" LINE ") check", "[checker][" CATEGORY "]") { \
    std::string line { LINE };						\
    auto parsed = parse(line);						\
    auto res = check(parsed);						\
    CHECKED_IF(res.get() == nullptr) {					\
      SUCCEED();							\
    }									\
    CHECKED_ELSE(res.get() == nullptr) {				\
      FAIL(res.get()->to_string());					\
    }									\
  }

#define CHECKER_ERROR(CATEGORY, LINE, ERRTYPE)				\
  TEST_CASE(CATEGORY " instruction (" LINE ")", "[checker][" CATEGORY "]") { \
    std::string line { LINE };						\
    auto parsed = parse(line);						\
    auto res = check(parsed);						\
    /* Check if error is reported */					\
    REQUIRE(res.get() != nullptr);					\
    /* Check if error is of correct type */				\
    REQUIRE_NOTHROW(dynamic_cast<ERRTYPE*>(res.get()));			\
  }

CHECKER_ERROR("Any", "UNKNOWN", UnrecognizedInstructionError);
CHECKER_ERROR("Any", "ADDI x1 x2", ArgCountError);

CHECKER_SUCCESS("I", "ADDI x1 x2 4095"); // 4095 is the largest immediate which should be acceptable
CHECKER_ERROR("I", "ADDI xbad x1 12", OperandTypeError);
CHECKER_ERROR("I", "ADDI x1 xbad 12", OperandTypeError);
CHECKER_ERROR("I", "ADDI x1 x1 bad", OperandTypeError);
CHECKER_ERROR("I", "ADDI x1 x1 4096", HighBitsLostError);
// with type I instruction immediate cannot be truncated
CHECKER_SUCCESS("R", "ADD x2 x4 x5");
CHECKER_ERROR("R", "ADD xbad x4 x5", OperandTypeError);
CHECKER_ERROR("R", "ADD x2 xbad x5", OperandTypeError);
CHECKER_ERROR("R", "ADD x2 x4 xbad", OperandTypeError);

CHECKER_SUCCESS("U", "LUI x1 lbl");
CHECKER_SUCCESS("U", "LUI x1 4294963200"); // all bits of an immediate which would not be truncated
CHECKER_ERROR("U", "LUI x1 4294963201", LowBitsLostError) // leftmost bit will would be truncated
