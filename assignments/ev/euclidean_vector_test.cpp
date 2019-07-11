
#include <sstream>

#include "assignments/ev/euclidean_vector.h"
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

#include "assignments/ev/euclidean_vector.h"
#include "catch.h"

SCENARIO("Testing Constructors and ostream") {
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
    AND_GIVEN("Some reference Vectors to compare with") {
      std::vector v1{0.0};
      std::vector v2{0.0, 0.0};
      std::vector v3{0.0, 0.0, 0.0, 0.0, 0.0};
      EuclideanVector ref_ev1{v1.begin(), v1.end()};
      EuclideanVector ref_ev2{v2.begin(), v2.end()};
      EuclideanVector ref_ev3{v3.begin(), v3.end()};

      AND_THEN("Their initial values should be 0.0") {
        REQUIRE(no_arg == ref_ev1);
        REQUIRE(one_pos == ref_ev1);
        REQUIRE(two_pos == ref_ev2);
        REQUIRE(five_pos == ref_ev3);
      }
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
  // Todo explain why ostream is tested here
  GIVEN("Some Vectors and Lists") {
    std::vector<double> v0{};
    std::vector<double> v1{1.0, 2.0, 3.0};
    std::vector<double> v2{1.5, -2.5, 3.5, -4.5, 5.5};
    std::list<double> l0{};
    std::list<double> l1{1.0, 2.0, 3.0};
    std::list<double> l2{1.5, -2.5, 3.5, -4.5, 5.5};
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
      WHEN("We try to print them out") {
        THEN("their output string should be the same") {
          std::stringstream out0;
          std::stringstream out1;
          std::stringstream out2;
          out0 << ev0;
          out1 << ev1;
          out2 << ev2;
          REQUIRE(out0.str() == "[ ]");
          REQUIRE(out1.str() == "[ 1 2 3 ]");
          REQUIRE(out2.str() == "[ 1.5 -2.5 3.5 -4.5 5.5 ]");
        }
      }
    }
  }
}

// after this point we are confident with our constructor is working
SCENARIO("Testing Methods") {
  GIVEN("Some Arrays") {

    std::vector<double> v0{};
    std::vector<double> v1{1.0, 2.0, 3.0};
    std::vector<double> v2{1.0, -2.0, 3.0, -4.0, 5.0};
    std::vector<double> v3{0.0, 0.0};
    AND_GIVEN("Some EV") {
      EuclideanVector ev0{v0.begin(), v0.end()};
      EuclideanVector ev1(v1.begin(), v1.end());
      EuclideanVector ev2(v2.begin(), v2.end());
      EuclideanVector ev3(v3.begin(), v3.end());
      WHEN("We try to Copy assign it") {
        EuclideanVector ev4{v1.begin(), v1.end()};  // we try to assign it to other variable
        THEN("e4 Should be equal to v0") {
          ev4 = ev0;
          REQUIRE(ev4 == ev0);
        }
        THEN("e4 Should be equal to ev1") {
          ev4 = ev1;
          REQUIRE(ev4 == ev1);
        }
        THEN("e4 Should be equal to ev2") {
          ev4 = ev2;
          REQUIRE(ev4 == ev2);
        }
        AND_WHEN("We Try to Chain Assign it") {
          ev4 = ev0 = ev1;
          REQUIRE(ev4 == ev1);
          // TODO explain why is this important
        }
      }

      AND_GIVEN("some reference vectors") {

        EuclideanVector ref0 = ev0;
        EuclideanVector ref1 = ev1;
        EuclideanVector ref2 = ev2;
        EuclideanVector ref3 = ev3;
        REQUIRE(ref0 == ev0);
        REQUIRE(ref1 == ev1);
        REQUIRE(ref2 == ev2);
        REQUIRE(ref3 == ev3);
        WHEN("We try to move assign it") {
          EuclideanVector ev4{v1.begin(), v1.end()};

          THEN("it should be equal to what it move assigned") {
            ev4 = std::move(ev0);
            REQUIRE(ev4 == ref0);
            REQUIRE(ev0.GetNumDimensions() == 0);
          }
          THEN("it should be equal to what it move assigned") {
            ev4 = std::move(ev1);
            REQUIRE(ev4 == ref1);
            REQUIRE(ev1.GetNumDimensions() == 0);
          }
          THEN("it should be equal to what it move assigned") {
            ev4 = std::move(ev2);
            REQUIRE(ev4 == ref2);
            REQUIRE(ev2.GetNumDimensions() == 0);
          }
          THEN("it should be equal to what it move assigned") {
            ev4 = std::move(ev3);
            REQUIRE(ev4 == ref3);
            REQUIRE(ev3.GetNumDimensions() == 0);
          }
          WHEN("We try to subscript on it") {
            double aa = {ev0[0]};
            double bb = {ev1[0]};
            double cc = {ev2[0]};
            double dd = {ev3[0]};

            REQUIRE(aa == ev0[0]);
            REQUIRE(bb == ev1[0]);
            REQUIRE(cc == ev2[0]);
            REQUIRE(dd == ev3[0]);

            ev0[0] = 2.0;
            ev1[0] = 2.0;
            ev1[1] = 2.0;
            ev2[0] = 2.0;
            ev0[1] = 2.0;
            ev3[1] = 2.0;
            ev3[0] = 2.0;

            REQUIRE(ev0[0] == 2.0);
            REQUIRE(ev1[0] == 2.0);
            REQUIRE(ev1[1] == 2.0);
            REQUIRE(ev2[0] == 2.0);

            REQUIRE(ev3[1] == 2.0);
            REQUIRE(ev3[0] == 2.0);
          }
          WHEN("We Try To Subscript on Invalid Element") {
            REQUIRE_FALSE(ev0[1] == 2.0);
            REQUIRE_FALSE(ev1[-1] == 2.0);
            REQUIRE_FALSE(ev2[-1] == 2.0);
            REQUIRE_FALSE(ev3[-1] == 2.0);
          }
        }
      }
      AND_GIVEN("Some Reference Number to test with") {
        double ref_zero = sqrt(0);
        double ref_aa = sqrt(1.0 * 1.0 + 2.0 * 2.0 + 3.0 * 3.0);
        double ref_bb = sqrt(1.0 * 1.0 + -2.0 * -2.0 + 3.0 * 3.0 + -4.0 * -4.0 + 5.0 * 5.0);
        WHEN("We call GetEuclideanNorm()") {
          double aa = ev1.GetEuclideanNorm();
          double bb = ev2.GetEuclideanNorm();
          double zero_1 = ev3.GetEuclideanNorm();
          THEN("It should give the right answers with Reference Numbers") {
            REQUIRE(aa == ref_aa);
            REQUIRE(bb == ref_bb);
            REQUIRE(zero_1 == ref_zero);
            REQUIRE_THROWS_WITH(ev0.GetEuclideanNorm(),
                                "EuclideanVector with no dimensions does not have a norm");
          }

          WHEN("We call CreateUnitVector()") {
            EuclideanVector result_1 = ev1.CreateUnitVector();
            EuclideanVector result_2 = ev2.CreateUnitVector();
            GIVEN("Some Reference Number to test with") {
              std::for_each(v1.begin(), v1.end(), [=](double& n) { n /= ref_aa; });
              std::for_each(v2.begin(), v2.end(), [=](double& n) { n /= ref_bb; });
              EuclideanVector ref_v1{v1.begin(), v1.end()};
              EuclideanVector ref_v2{v2.begin(), v2.end()};
              THEN("It should give the right answers with Reference Numbers") {
                REQUIRE(result_1 == ref_v1);
                REQUIRE(result_2 == ref_v2);
                REQUIRE_THROWS_WITH(
                    ev3.CreateUnitVector(),
                    "EuclideanVector with euclidean normal of 0 does not have a unit vector");
                REQUIRE_THROWS_WITH(
                    ev0.CreateUnitVector(),
                    "EuclideanVector with no dimensions does not have a unit vector");
              }
            }
            THEN("The old variables should be the same") {
              EuclideanVector tmp_ev1{v1.begin(), v1.end()};
              EuclideanVector tmp_ev2{v2.begin(), v2.end()};
              REQUIRE(ev1 == tmp_ev1);
              REQUIRE(ev2 == tmp_ev2);
            }
          }
        }
      }
    }
  }
}

SCENARIO("Friends test cases") {
  GIVEN("Some Vectors ") {
    std::vector<double> v0{};
    std::vector<double> v1{1.0, 2.0, 3.0};
    std::vector<double> v2{1.0, -2.0, 3.0, -4.0, 5.0};
    std::vector<double> v3{0.0, 0.0};
    AND_GIVEN("Some EV") {
      EuclideanVector ev0{v0.begin(), v0.end()};
      EuclideanVector ev1(v1.begin(), v1.end());
      EuclideanVector ev2(v2.begin(), v2.end());
      EuclideanVector ev3(v3.begin(), v3.end());
      AND_GIVEN("we initialise the same Euclidean Vector") {
        EuclideanVector same_0 = ev0;
        EuclideanVector same_1 = ev1;
        EuclideanVector same_2 = ev2;
        EuclideanVector same_3 = ev3;
        WHEN("We try to compare each of these") {
          THEN("It should give true and pass the tests")
          REQUIRE(same_0 == ev0);
          REQUIRE(same_1 == ev1);
          REQUIRE(same_2 == ev2);
          REQUIRE(same_3 == ev3);
        }
        WHEN("We try to compare them incorrectly") {
          THEN("it should give false") {
            REQUIRE_FALSE(same_0 == ev1);
            REQUIRE_FALSE(same_1 == ev3);
            REQUIRE_FALSE(same_3 == ev2);
            REQUIRE(same_0 != ev2);
            REQUIRE(same_1 != ev0);
          }
        }
      }
      AND_GIVEN("We initialise some reference vectors for the operation and vectors to do the "
                "operation") {

        EuclideanVector sum_ev0(0);
        EuclideanVector sum_ev1(3, 3.0);
        EuclideanVector sum_ev2(5, 5.0);
        EuclideanVector sum_ev3(2, 8.0);

        double test_num_1 = 3.0;
        double test_num_2 = 5.0;
        double test_num_3 = 8.0;
        std::for_each(v1.begin(), v1.end(), [=](double& n) { n += test_num_1; });
        std::for_each(v2.begin(), v2.end(), [=](double& n) { n += test_num_2; });
        std::for_each(v3.begin(), v3.end(), [=](double& n) { n += test_num_3; });
        EuclideanVector ref_v1{v1.begin(), v1.end()};
        EuclideanVector ref_v2{v2.begin(), v2.end()};
        EuclideanVector ref_v3{v3.begin(), v3.end()};

        WHEN("we try to do addition") {
          EuclideanVector result_0 = sum_ev0 + ev0;
          EuclideanVector result_1 = sum_ev1 + ev1;
          EuclideanVector result_2 = sum_ev2 + ev2;
          EuclideanVector result_3 = sum_ev3 + ev3;
          THEN("We should get the same results from our reference vectors") {
            REQUIRE(ev0 == result_0);
            REQUIRE(ref_v1 == result_1);
            REQUIRE(ref_v2 == result_2);
            REQUIRE(ref_v3 == result_3);
          }
        }
        AND_WHEN("We try to do invalid operation") {
          THEN("it should throw an exception") {
            REQUIRE_THROWS_WITH(sum_ev1 + ev0, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(sum_ev1 + ev2, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(sum_ev1 + ev0, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(sum_ev2 + ev1, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(sum_ev1 + ev3, "Dimensions of LHS(X) and RHS(Y) do not match");
          }
        }
      }

      AND_GIVEN("We initialise some reference vectors for the operation and vectors to do the "
                "operation") {
        EuclideanVector sub_ev0(0);
        EuclideanVector sub_ev1(3, 3.0);
        EuclideanVector sub_ev2(5, 5.0);
        EuclideanVector sub_ev3(2, 8.0);

        double test_num_1 = 3.0;
        double test_num_2 = 5.0;
        double test_num_3 = 8.0;
        std::for_each(v1.begin(), v1.end(), [=](double& n) { n -= test_num_1; });
        std::for_each(v2.begin(), v2.end(), [=](double& n) { n -= test_num_2; });
        std::for_each(v3.begin(), v3.end(), [=](double& n) { n -= test_num_3; });
        EuclideanVector ref_v1{v1.begin(), v1.end()};
        EuclideanVector ref_v2{v2.begin(), v2.end()};
        EuclideanVector ref_v3{v3.begin(), v3.end()};

        WHEN("we try to do subtraction") {
          EuclideanVector result_0 = ev0 - sub_ev0;
          EuclideanVector result_1 = ev1 - sub_ev1;
          EuclideanVector result_2 = ev2 - sub_ev2;
          EuclideanVector result_3 = ev3 - sub_ev3;
          THEN("We should get the same results from our reference vectors") {
            REQUIRE(ev0 == result_0);
            REQUIRE(ref_v1 == result_1);
            REQUIRE(ref_v2 == result_2);
            REQUIRE(ref_v3 == result_3);
          }
        }
        AND_WHEN("We try to do invalid operation") {
          THEN("it should throw an exception") {
            REQUIRE_THROWS_WITH(sub_ev0 - ev2, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(sub_ev1 - ev2, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(sub_ev1 - ev0, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(sub_ev2 - ev1, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(sub_ev1 - ev3, "Dimensions of LHS(X) and RHS(Y) do not match");
          }
        }
      }

      AND_GIVEN("We initialise some reference vectors for the operation and vectors to do the "
                "operation") {
        EuclideanVector mul_ev1(3, 3.0);
        EuclideanVector mul_ev2(5, 5.0);
        EuclideanVector mul_ev3(2, 8.0);

        double test_num_1 = 1.0 * 3.0 + 2.0 * 3.0 + 3.0 * 3.0;
        double test_num_2 = 1.0 * 5.0 + -2.0 * 5.0 + 3.0 * 5.0 + -4.0 * 5.0 + 5.0 * 5.0;
        double test_num_3 = 0.0;

        WHEN("we try to do subtraction") {
          // NOT TESTED double result_0{ev0 * mul_ev0};
          double result_1{ev1 * mul_ev1};
          double result_2{ev2 * mul_ev2};
          double result_3{ev3 * mul_ev3};
          THEN("We should get the same results from our reference vectors") {
            REQUIRE(test_num_1 == result_1);
            REQUIRE(test_num_2 == result_2);
            REQUIRE(test_num_3 == result_3);
          }
        }
        AND_WHEN("We try to do invalid operation") {
          THEN("it should throw an exception") {
            REQUIRE_THROWS_WITH(ev0 * mul_ev1, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(ev2 * mul_ev1, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(ev1 * mul_ev2, "Dimensions of LHS(X) and RHS(Y) do not match");
            REQUIRE_THROWS_WITH(ev3 * mul_ev1, "Dimensions of LHS(X) and RHS(Y) do not match");
          }
        }
      }
      AND_GIVEN("We initialise some reference vectors for the operation and vectors to do the "
                "operation") {

        double test_num_0 = 0;
        double test_num_1 = -1;
        double test_num_2 = 2;
        double test_num_3 = -3;
        double test_num_4 = 4;
        EuclideanVector ev_result_0 = ev0;
        EuclideanVector ev_result_1 = ev1;
        EuclideanVector ev_result_2 = ev2;
        EuclideanVector ev_result_3 = ev3;
        EuclideanVector ev_result_4 = ev1;
        EuclideanVector ev_result_5 = ev2;
        EuclideanVector ev_result_6 = ev3;

        ev_result_0 *= test_num_0;
        ev_result_1 *= test_num_1;
        ev_result_2 *= test_num_2;
        ev_result_3 *= test_num_3;
        ev_result_4 *= test_num_4;
        ev_result_5 *= test_num_3;
        ev_result_6 *= test_num_1;
        WHEN("we try to do multiply operation (ev * double)") {
          EuclideanVector result_0 = ev0 * test_num_0;
          EuclideanVector result_1 = ev1 * test_num_1;
          EuclideanVector result_2 = ev2 * test_num_2;
          EuclideanVector result_3 = ev3 * test_num_3;
          EuclideanVector result_4 = ev1 * test_num_4;
          EuclideanVector result_5 = ev2 * test_num_3;
          EuclideanVector result_6 = ev3 * test_num_1;

          THEN("We should get the same results from our reference vectors") {
            REQUIRE(ev_result_0 == result_0);
            REQUIRE(ev_result_1 == result_1);
            REQUIRE(ev_result_2 == result_2);
            REQUIRE(ev_result_3 == result_3);
            REQUIRE(ev_result_4 == result_4);
            REQUIRE(ev_result_5 == result_5);
            REQUIRE(ev_result_6 == result_6);
          }
        }
        WHEN("we try to do multiply operation (double * ev)") {
          EuclideanVector result_0 = test_num_0 * ev0;
          EuclideanVector result_1 = test_num_1 * ev1;
          EuclideanVector result_2 = test_num_2 * ev2;
          EuclideanVector result_3 = test_num_3 * ev3;
          EuclideanVector result_4 = test_num_4 * ev1;
          EuclideanVector result_5 = test_num_3 * ev2;
          EuclideanVector result_6 = test_num_1 * ev3;

          THEN("We should get the same results from our reference vectors") {
            REQUIRE(ev_result_0 == result_0);
            REQUIRE(ev_result_1 == result_1);
            REQUIRE(ev_result_2 == result_2);
            REQUIRE(ev_result_3 == result_3);
            REQUIRE(ev_result_4 == result_4);
            REQUIRE(ev_result_5 == result_5);
            REQUIRE(ev_result_6 == result_6);
          }
        }
      }
      AND_GIVEN("We initialise some reference vectors for the operation and vectors to do the "
                "operation") {

        double test_num_1 = 3.0;
        double test_num_2 = 5.0;
        double test_num_3 = 8.0;
        std::for_each(v1.begin(), v1.end(), [=](double& n) { n /= test_num_1; });
        std::for_each(v2.begin(), v2.end(), [=](double& n) { n /= test_num_2; });
        std::for_each(v3.begin(), v3.end(), [=](double& n) { n /= test_num_3; });
        EuclideanVector ref_v1{v1.begin(), v1.end()};
        EuclideanVector ref_v2{v2.begin(), v2.end()};
        EuclideanVector ref_v3{v3.begin(), v3.end()};

        WHEN("we try to do division") {
          EuclideanVector result_1 = ev1 / test_num_1;
          EuclideanVector result_2 = ev2 / test_num_2;
          EuclideanVector result_3 = ev3 / test_num_3;
          THEN("We should get the same results from our reference vectors") {
            REQUIRE(ref_v1 == result_1);
            REQUIRE(ref_v2 == result_2);
            REQUIRE(ref_v3 == result_3);
          }
        }
        AND_WHEN("We try to do invalid operation") {
          THEN("it should throw an exception") {
            REQUIRE_THROWS_WITH(ev0 / 0.0, "Invalid vector division by 0");
            REQUIRE_THROWS_WITH(ev1 / 0, "Invalid vector division by 0");
            REQUIRE_THROWS_WITH(ev2 / 0, "Invalid vector division by 0");
            REQUIRE_THROWS_WITH(ev3 / 0, "Invalid vector division by 0");
          }
        }
      }
    }
  }
}
