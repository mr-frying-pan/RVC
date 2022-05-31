#include <catch2/catch.hpp>
#include <cstdint>
#include "../constants.hpp"

#define BITS_CHECK(TYPE, SIZE, VALUE)				\
  SECTION("Mask of " #SIZE " " #TYPE " is " #VALUE) {		\
    auto mask = constants::bitmasks::TYPE(SIZE);		\
    REQUIRE(mask == static_cast<uint32_t>(VALUE));		\
  }

TEST_CASE("lowBits test case", "[utils]") {
  BITS_CHECK(lowBits, 0, 0);
  BITS_CHECK(lowBits, 32, ~0);
  BITS_CHECK(lowBits, 40, ~0);
  BITS_CHECK(lowBits, 1, 1);
  BITS_CHECK(lowBits, 31, ~(static_cast<uint32_t>(1) << 31));
}

TEST_CASE("highBits test case", "[utils]") {
  BITS_CHECK(highBits, 0, 0);
  BITS_CHECK(highBits, 32, ~0);
  BITS_CHECK(highBits, 40, ~0);
  BITS_CHECK(highBits, 1, static_cast<uint32_t>(1) << 31);
  BITS_CHECK(highBits, 31, ~1);
}
