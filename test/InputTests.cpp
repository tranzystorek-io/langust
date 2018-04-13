#include <sstream>

#include "Input.hpp"
#include "catch.hpp"

TEST_CASE( "input is read", "[input]" )
{
  std::istringstream iss("abc");
  langust::parse::Input input(iss);

  REQUIRE( input.getNextChar() == 'a' );

  REQUIRE( input.getNextChar() == 'b' );

  REQUIRE( input.getNextChar() == 'c' );
}
