#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

TEST_CASE("Checking framework (pass)", "[what-is-this]") {
  REQUIRE(1 == 1);
}

TEST_CASE("Checking framework (fail)", "[what-is-this]") {
  REQUIRE(0 == 1);
}
