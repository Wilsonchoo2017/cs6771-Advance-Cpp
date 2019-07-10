/*

  == Explanation and rational of testing ==

  Choice of values (COV) to test:
    - 0
    - so of small values that is close to 0:  1, 2, 5,
    Max and min is not used due to errors such as these are due to not enough memory in the VM.
  Thus, they are removed. (also makes testing very slow D:)

  Approach of testing: (White Box)
    We will test each public methods and friends with our COV unless specified not to. Purpose of
    this is to have large coverage and includes our edge cases.
  Const Correctness: TODO


   In Construction Test Case:
   GetNumDiemsion, (one of) At, ==, vector & list casting is used thus some of these test are
  skipped

*/

#include <limits.h>

#include "assignments/ev/euclidean_vector.h"
#include "catch.h"

bool check_if_content_is_zero(EuclideanVector vec) {
  for (int i = 0; i < vec.GetNumDimensions(); i++) {
    if (vec.at(i) != 0.0) {
      return false;
    }
  }
  return true;
}

SCENARIO("Testing Constructors") {
  GIVEN("some default constructor") {
    // According to Spec default constructor will always be non-negative and no non-argument test
    EuclideanVector no_arg;  // Not tested but good to have
    EuclideanVector one_pos(1);
    EuclideanVector two_pos(2);
    EuclideanVector five_pos(5);
    EuclideanVector zero(0);  // z for zero
    THEN("All the Dimension should set appropriately according to input value") {
      REQUIRE(no_arg.GetNumDimensions() == 1);
      REQUIRE(one_pos.GetNumDimensions() == 1);
      REQUIRE(two_pos.GetNumDimensions() == 2);
      REQUIRE(five_pos.GetNumDimensions() == 5);
      REQUIRE(zero.GetNumDimensions() == 0);
    }
    AND_THEN("Their initial values should be 0.0") {
      REQUIRE(check_if_content_is_zero(no_arg));
      REQUIRE(check_if_content_is_zero(one_pos));
      REQUIRE(check_if_content_is_zero(two_pos));
      REQUIRE(check_if_content_is_zero(five_pos));
      REQUIRE(check_if_content_is_zero(zero));
    }
  }
  GIVEN("Some Constructors with arguments (int and double)") {
    // All int is non-zero
    EuclideanVector one_pos(1, 1.1111);
    EuclideanVector one_neu(1, 0.0);
    EuclideanVector one_neg(1, -1.1111);
    EuclideanVector two_pos(2, 2.2222);
    EuclideanVector two_neu(2, 0.0);
    EuclideanVector two_neg(2, -2.2222);
    EuclideanVector five_pos(5, 5.5555);
    EuclideanVector five_neu(5, 0.0);
    EuclideanVector five_neg(5, -5.5555);
    EuclideanVector zero_pos(0, 1.1);
    EuclideanVector zero_neu(0, 0.0);
    EuclideanVector zero_neg(0, -1.1);
    THEN("All the array should be Dimensionally valid") {
      REQUIRE(one_pos.GetNumDimensions() == 1);
      REQUIRE(one_neu.GetNumDimensions() == 1);
      REQUIRE(one_neg.GetNumDimensions() == 1);
      REQUIRE(two_pos.GetNumDimensions() == 2);
      REQUIRE(two_neu.GetNumDimensions() == 2);
      REQUIRE(two_neg.GetNumDimensions() == 2);
      REQUIRE(five_pos.GetNumDimensions() == 5);
      REQUIRE(five_neu.GetNumDimensions() == 5);
      REQUIRE(five_neg.GetNumDimensions() == 5);
      REQUIRE(zero_pos.GetNumDimensions() == 0);
      REQUIRE(zero_neu.GetNumDimensions() == 0);
      REQUIRE(zero_neg.GetNumDimensions() == 0);
    }
    AND_THEN("All their initialisation should be accordingly as specified") {
      GIVEN("Some Reference EucliVector by copy Constructor and"
            " Constructor(it, it) for dimension one vectors") {
        std::vector<double> vec1_pos{1.1111};
        std::vector<double> vec1_neu{0.0};
        std::vector<double> vec1_neg{-1.1111};
        EuclideanVector expectedv1_pos(vec1_pos.begin(), vec1_pos.end());
        EuclideanVector expectedv1_neu(vec1_neu.begin(), vec1_neu.end());
        EuclideanVector expectedv1_neg(vec1_neg.begin(), vec1_neg.end());
        EuclideanVector expectedv1_pos2{expectedv1_pos};
        EuclideanVector expectedv1_neu2{expectedv1_neu};
        EuclideanVector expectedv1_neg2{expectedv1_neg};

        EuclideanVector expectedv1_neu3(1);
        WHEN("Compared with expected vector, they should return true") {
          REQUIRE(one_pos == expectedv1_pos);
          REQUIRE(one_neu == expectedv1_neu);
          REQUIRE(one_neg == expectedv1_neg);

          REQUIRE(one_pos == expectedv1_pos2);
          REQUIRE(one_neu == expectedv1_neu2);
          REQUIRE(one_neg == expectedv1_neg2);

          REQUIRE(one_neu == expectedv1_neu3);
        }
        WHEN("Compared with wrong vector, they should return false") {
          REQUIRE_FALSE(one_pos ==
                        expectedv1_neu);  // Just proving that expected is same as one_neu
          REQUIRE_FALSE(one_pos == one_neg);
        }
        WHEN("We try to move construct") {
          EuclideanVector moved_v1_pos{std::move(one_pos)};
          EuclideanVector moved_v1_neu{std::move(one_neu)};
          EuclideanVector moved_v1_neg{std::move(one_neg)};
          THEN("Our old should be 0 and new should be the old one") {
            REQUIRE(one_pos.GetNumDimensions() == 0);
            REQUIRE(one_neu.GetNumDimensions() == 0);
            REQUIRE(one_neg.GetNumDimensions() == 0);

            REQUIRE(moved_v1_pos == expectedv1_pos);
            REQUIRE(moved_v1_neu == expectedv1_neu);
            REQUIRE(moved_v1_neg == expectedv1_neg);
          }
        }
      }
      GIVEN("Some Reference EucliVector by Constructor(it, it) for dimension two vectors") {
        std::vector<double> vec2_pos{2.2222, 2.2222};
        std::vector<double> vec2_neu{0.0, 0.0};
        std::vector<double> vec2_neg{-2.2222, -2.2222};
        EuclideanVector expectedv2_pos(vec2_pos.begin(), vec2_pos.end());
        EuclideanVector expectedv2_neu(vec2_neu.begin(), vec2_neu.end());
        EuclideanVector expectedv2_neg(vec2_neg.begin(), vec2_neg.end());

        EuclideanVector expectedv2_pos2{expectedv2_pos};
        EuclideanVector expectedv2_neu2{expectedv2_neu};
        EuclideanVector expectedv2_neg2{expectedv2_neg};

        EuclideanVector expectedv2_neu3(2);
        WHEN("Compared with expected vector, they should return true") {
          REQUIRE(two_pos == expectedv2_pos);
          REQUIRE(two_neu == expectedv2_neu);
          REQUIRE(two_neu == expectedv2_neu2);
          REQUIRE(two_neg == expectedv2_neg);
        }
        WHEN("Compared with wrong vector, they should return false") {
          REQUIRE_FALSE(two_pos == expectedv2_neu);
          REQUIRE_FALSE(two_pos == two_neg);
          REQUIRE_FALSE(two_pos == one_pos);
          REQUIRE_FALSE(two_neg == one_neu);
          REQUIRE_FALSE(two_neg == one_neg);
        }
        WHEN("We try to move construct") {
          EuclideanVector moved_v2_pos{std::move(two_pos)};
          EuclideanVector moved_v2_neu{std::move(two_neu)};
          EuclideanVector moved_v2_neg{std::move(two_neg)};
          THEN("Our old should be 0 and new should be the old one") {
            REQUIRE(two_pos.GetNumDimensions() == 0);
            REQUIRE(two_neu.GetNumDimensions() == 0);
            REQUIRE(two_neg.GetNumDimensions() == 0);

            REQUIRE(moved_v2_pos == expectedv2_pos);
            REQUIRE(moved_v2_neu == expectedv2_neu);
            REQUIRE(moved_v2_neg == expectedv2_neg);
          }
        }
      }

      GIVEN("Some Reference EucliVector by Constructor(it, it) for dimension five vectors") {
        std::vector<double> vec5_pos{5.5555, 5.5555, 5.5555, 5.5555, 5.5555};
        std::vector<double> vec5_neu{0.0, 0.0, 0.0, 0.0, 0.0};
        std::vector<double> vec5_neg{-5.5555, -5.5555, -5.5555, -5.5555, -5.5555};
        EuclideanVector expectedv5_pos(vec5_pos.begin(), vec5_pos.end());
        EuclideanVector expectedv5_neu(vec5_neu.begin(), vec5_neu.end());
        EuclideanVector expectedv5_neu2(5);
        EuclideanVector expectedv5_neg(vec5_neg.begin(), vec5_neg.end());
        WHEN("Compared with expected vector, they should return true") {
          REQUIRE(five_pos == expectedv5_pos);
          REQUIRE(five_neu == expectedv5_neu);
          REQUIRE(five_neu == expectedv5_neu2);
          REQUIRE(five_neg == expectedv5_neg);
        }
        WHEN("Compared with wrong vector, they should return false") {
          REQUIRE_FALSE(five_pos == one_neg);
          REQUIRE_FALSE(five_pos == expectedv5_neu);
          REQUIRE_FALSE(five_pos == two_neg);
          REQUIRE_FALSE(five_pos == one_pos);
          REQUIRE_FALSE(five_neg == one_neu);
          REQUIRE_FALSE(five_neg == one_neg);
          REQUIRE_FALSE(five_neu == five_neg);
          REQUIRE_FALSE(five_neu == five_pos);
          REQUIRE_FALSE(five_neg == two_neu);
        }
      }
      GIVEN("Some Reference EucliVector by Constructor(it, it) for dimension zero vectors") {
        std::vector<double> vec0_pos{};
        std::vector<double> vec0_neu{};
        std::vector<double> vec0_neg{};
        EuclideanVector expectedv0_pos(vec0_pos.begin(), vec0_pos.end());
        EuclideanVector expectedv0_neu(vec0_neu.begin(), vec0_neu.end());
        EuclideanVector expectedv0_neu2(5);
        EuclideanVector expectedv0_neg(vec0_neg.begin(), vec0_neg.end());
        WHEN("Compared with expected vector, they should return true") {
          REQUIRE(zero_pos == expectedv0_pos);
          REQUIRE(zero_neu == expectedv0_neu);
          REQUIRE(zero_neg == expectedv0_neg);

          REQUIRE(zero_pos == expectedv0_neu);  // semantically they should be the same
        }
        WHEN("Compared with wrong vector, they should return false") {
          REQUIRE_FALSE(zero_neu == expectedv0_neu2);
          REQUIRE_FALSE(zero_pos == one_neg);
          REQUIRE_FALSE(zero_pos == two_neg);
          REQUIRE_FALSE(zero_pos == one_pos);
          REQUIRE_FALSE(zero_neg == one_neu);
          REQUIRE_FALSE(zero_neg == one_neg);
          REQUIRE_FALSE(zero_neu == five_neg);
          REQUIRE_FALSE(zero_neu == five_pos);
          REQUIRE_FALSE(zero_neg == two_neu);
        }
        WHEN("We try to move construct") {
          EuclideanVector moved_v0_pos{std::move(zero_pos)};
          EuclideanVector moved_v0_neu{std::move(zero_neu)};
          EuclideanVector moved_v0_neg{std::move(zero_neg)};
          THEN("Our old should be 0 and new should be the old one") {
            REQUIRE(zero_pos.GetNumDimensions() == 0);
            REQUIRE(zero_neu.GetNumDimensions() == 0);
            REQUIRE(zero_neg.GetNumDimensions() == 0);

            REQUIRE(moved_v0_pos == expectedv0_pos);
            REQUIRE(moved_v0_neu == expectedv0_neu);
            REQUIRE(moved_v0_neg == expectedv0_neg);
          }
        }
      }
    }
  }
  GIVEN("Some Vectors and Lists") {
    std::vector<double> v0{};
    std::vector<double> v1{1.0, 2.0, 3.0};
    std::vector<double> v2{1.0, -2.0, 3.0, -4.0, 5.0};
    std::list<double> l0{};
    std::list<double> l1{1.0, 2.0, 3.0};
    std::list<double> l2{1.0, -2.0, 3.0, -4.0, 5.0};
    AND_GIVEN("Euclid initialised by these Vectors to compare with") {
      EuclideanVector ev0{v0.begin(), v0.end()};
      EuclideanVector ev1(v1.begin(), v1.end());
      EuclideanVector ev2(v2.begin(), v2.end());

      WHEN("We cast them") {
        auto sc_ev0 = std::vector<double>{ev0};
        auto sc_ev1 = std::vector<double>{ev1};
        auto sc_ev2 = std::vector<double>{ev2};

        auto sc_ev3 = std::list<double>{ev0};
        auto sc_ev4 = std::list<double>{ev1};
        auto sc_ev5 = std::list<double>{ev2};
        THEN("They should give the same vectors") {
          REQUIRE(sc_ev0 == v0);
          REQUIRE(sc_ev1 == v1);
          REQUIRE(sc_ev2 == v2);
          REQUIRE(sc_ev3 == l0);
          REQUIRE(sc_ev4 == l1);
          REQUIRE(sc_ev5 == l2);
        }
      }
    }
  }
}

// after this point we are confident with our constructor is working

TEST_CASE("Testing Methods") {
  GIVEN("Some Arrays") {

    std::vector<double> v0{};
    std::vector<double> v1{1.0, 2.0, 3.0};
    std::vector<double> v2{1.0, -2.0, 3.0, -4.0, 5.0};
    AND_GIVEN("Some EV") {

      EuclideanVector ev0{v0.begin(), v0.end()};
      EuclideanVector ev1(v1.begin(), v1.end());
      EuclideanVector ev2(v2.begin(), v2.end());
      WHEN("We try to Copy assign it") {
        EuclideanVector ev3{v1.begin(), v1.end()};
        THEN("e3 Should be equal to v0") {
          ev3 = ev0;
          REQUIRE(ev3 == ev0);
        }
        THEN("e3 Should be equal to ev1") {
          ev3 = ev1;
          REQUIRE(ev3 == ev1);
        }
        THEN("e3 Should be equal to ev2") {
          ev3 = ev2;
          REQUIRE(ev3 == ev2);
        }
      }
      AND_GIVEN("some reference vectors") {

        EuclideanVector ref0 = ev0;
        EuclideanVector ref1 = ev1;
        EuclideanVector ref2 = ev2;
        REQUIRE(ref0 == ev0);
        REQUIRE(ref1 == ev1);
        REQUIRE(ref2 == ev2);
        WHEN("We try to copy assign it") {
          EuclideanVector ev3{v1.begin(), v1.end()};

          THEN("it should be equal to what it copy assigned") {
            ev3 = std::move(ev0);
            REQUIRE(ev3 == ref0);
            REQUIRE(ev0.GetNumDimensions() == 0);
          }
          THEN("it should be equal to what it copy assigned") {
            ev3 = std::move(ev1);
            REQUIRE(ev3 == ref1);
            REQUIRE(ev1.GetNumDimensions() == 0);
          }
          THEN("it should be equal to what it copy assigned") {
            ev3 = std::move(ev2);
            REQUIRE(ev3 == ref2);
            REQUIRE(ev2.GetNumDimensions() == 0);
          }
          WHEN("We try to subscript on it") {
            double aa = {ev0[0]};
            double bb = {ev1[0]};
            double cc = {ev2[0]};

            REQUIRE(aa == ev0[0]);
            REQUIRE(bb == ev1[0]);
            REQUIRE(cc == ev2[0]);

            ev0[0] = 2.0;
            ev1[0] = 2.0;
            ev1[1] = 2.0;
            ev2[0] = 2.0;
ev0[1] = 2.0;

            REQUIRE(ev0[0] == 2.0);
            REQUIRE(ev1[0] == 2.0);
            REQUIRE(ev1[1] == 2.0);
            REQUIRE(ev2[0] == 2.0);
            REQUIRE_FALSE(ev0[1] == 2.0);
          }
        }
      }
      AND_GIVEN("Some const EV") {
        EuclideanVector const ev9{v0.begin(), v0.end()};
        EuclideanVector const ev8(v1.begin(), v1.end());
        EuclideanVector const ev7(v2.begin(), v2.end());

        // TODO try to change value of auto const value
        WHEN("We try to Copy assign it") {
          EuclideanVector ev3{v1.begin(), v1.end()};
          THEN("e3 Should be equal to v9") {
            ev3 = ev9;
            REQUIRE(ev3 == ev9);
          }
          THEN("e3 Should be equal to ev8") {
            ev3 = ev8;
            REQUIRE(ev3 == ev8);
          }
          THEN("e3 Should be equal to ev7") {
            ev3 = ev7;
            REQUIRE(ev3 == ev7);
          }
        }
      }
    }
  }
}