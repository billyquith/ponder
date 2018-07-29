#pragma once

#include "../catch.hpp"

#include <ponder/value.hpp>
#include <ponder/enum.hpp>

// see https://github.com/philsquared/Catch/blob/master/docs/tostring.md

// Catch REQUIRE(x == y) converts x & y to strings so it can report errors. This causes
// a problem if x or y don't convert, so use ((x == y) == true) instead.
#define IS_TRUE(T)      REQUIRE((T))
#define IS_FALSE(T)     REQUIRE_FALSE((T))
#define IS_EQUAL(X,Y)   REQUIRE(X == Y)

inline std::ostream& operator << (std::ostream& stream, const ponder::Value& value)
{
    return stream << value.to<std::string>();
}
