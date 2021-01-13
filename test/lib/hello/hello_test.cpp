#include "doctest/doctest.h"

#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING

TEST_CASE ("hello") {
            CHECK_EQ("Tai", "Tai");
}
