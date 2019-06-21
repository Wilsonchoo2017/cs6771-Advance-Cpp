/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include "assignments/wl/word_ladder.h"
#include "catch.h"
#include <set>



// White Box Testing
SCENARIO("Simple Test 1: simply runs the program that has the only 2 and 3 nodes solution.") {
  GIVEN("A dictionary with specific words") {
    std::unordered_set<std::string> lexicon;
    lexicon.insert("can");
    lexicon.insert("cat");
    WHEN("Find Ladder is called it should return 2 nodes") {
      auto solution = FindLadder("can", "cat", lexicon);
      ListOfString referenced_solution = {{"can", "cat"}};
      REQUIRE(solution == referenced_solution);
    }
    GIVEN("An additional node into the dictionary") {
     lexicon.insert("con");
      WHEN("Find Ladder is called it should return 3 nodes") {
        auto solution = FindLadder("cat", "con", lexicon);
        ListOfString referenced_solution = {{"cat", "can", "con"}};
        REQUIRE(solution == referenced_solution);
      }
    }
  }


}

SCENARIO("Test 2: Cycle Check and Multi Path Solutions Check") {
  GIVEN("A dictionary with specific words that can cause cyclic paths") {
    std::unordered_set<std::string> lexicon;
    std::vector<std::string> tmp_vector({"cat", "cot", "cog", "bog", "bat", "con"});
    lexicon.insert(tmp_vector.begin(), tmp_vector.end());

    WHEN("Find Ladder is called") {
      auto solution = FindLadder("cat", "con", lexicon);
      ListOfString referenced_solution = {{"cat", "cot", "con"}};
      THEN("it should return 1 path with 3 nodes") {
        REQUIRE(solution == referenced_solution);
      }
    }
    GIVEN("Additional words that causes 2 paths to occur") {
      lexicon.insert("can");
      WHEN("Find Ladder is called") {
        auto solution = FindLadder("cat", "con", lexicon);
        std::sort(solution.begin(), solution.end());
        ListOfString referenced_solution = {{"cat", "can", "con"}, {"cat", "cot", "con"}};
        THEN("it should return 2 path with 3 nodes"){
          REQUIRE(solution == referenced_solution);
        }
      }
    }
  }
}

SCENARIO("Test 3: Correctness in populated dictionary to check whether it will ignore unrelated "
         "words") {
  GIVEN("A populated Dictioanry") {
    std::unordered_set<std::string> lexicon;
    std::vector<std::string> relevant_data({"cat", "cot", "cog", "bog", "bat", "con", "can", "cam"});
    lexicon.insert(relevant_data.begin(), relevant_data.end());
    std::vector<std::string> junk_data({"aa", "bb", "cc", "eeee", "zzzz", "aese", "fdae"});
    lexicon.insert(junk_data.begin(), junk_data.end());

    WHEN("Find Ladder is Called") {
      auto solution = FindLadder("cat", "con", lexicon);
      std::sort(solution.begin(), solution.end());
      ListOfString referenced_solution = {{"cat", "can", "con"}, {"cat", "cot", "con"}};
      THEN("it should return 2 path with 3 nodes") {
        REQUIRE(solution == referenced_solution);
      }
    }
  }
}





