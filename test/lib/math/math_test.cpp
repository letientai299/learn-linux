#include "doctest/doctest.h"

extern "C" {
#include "lib/math/math.h" // NOLINT(modernize-deprecated-headers)
}



TEST_CASE ("add") {
            CHECK(add(3, 4) == 7);
}

TEST_CASE ("sub") {
            CHECK(sub(3, 4) == -1);
}

TEST_CASE ("mul") {
            CHECK(mul(3, 4) == 12);
}
