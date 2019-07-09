/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include "assignments/ev/euclidean_vector.h"
#include "catch.h"


// ====== Constructor Test ========
SCENARIO("Testing Constructor") {
  GIVEN("some default constructor") {
    EuclideanVector a(1);
    EuclideanVector b(4);
    EuclideanVector c(100);
    THEN("All the Dimension should set appropriately and magnitude inside should be 0.0") {
      REQUIRE(a.dimen == 0);
      REQUIRE(a.);
    }
  }
}

