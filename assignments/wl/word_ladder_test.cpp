/*

  == Explanation and rational of testing ==
  My approach to testing is going from simple testing to more complex testing. This is so that in the
  early stages of my development I can make sure that my program doesn't fail in simple edge cases
  and leaving more time to debug potentially more complex bugs.

  Correctness: I used the reference solution and use its results to make my test scenarios. Thus
            Correctness shouldn't be an issue.
  Coverage: My test covers: no Path/single path with little nodes, branch checks, Cycle checks,
            Multi Path solution Check, and including a more complex test scenario. These tests are the usually the edge cases when dealing with graph nodes thus should be able to cover all edge cases.

*/

#include <algorithm>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "assignments/wl/word_ladder.h"
#include "catch.h"

SCENARIO("Simple Test 1: simply runs the program that has the only 0, 2 and 3 nodes solution.") {
  GIVEN("A dictionary with specific words") {
    std::unordered_set<std::string> lexicon;
    lexicon.insert("can");
    lexicon.insert("cat");
    lexicon.insert("den");
    WHEN("Find Ladder is called with solution") {
      auto solution = FindLadder("can", "cat", lexicon);
      ListOfString referenced_solution = {{"can", "cat"}};
      THEN("it should return 2 nodes") { REQUIRE(solution == referenced_solution); }
    }
    WHEN("Find Ladder is called with no solution") {
      auto solution = FindLadder("can", "den", lexicon);
      THEN("it should return 2 nodes") { REQUIRE(solution.empty()); }
    }

    WHEN("Find Ladder is called") {
      GIVEN("An additional node into the dictionary") {
        lexicon.insert("con");
        WHEN("Find Ladder is called ") {
          auto solution = FindLadder("cat", "con", lexicon);
          ListOfString referenced_solution = {{"cat", "can", "con"}};
          THEN("it should return 3 nodes") { REQUIRE(solution == referenced_solution); }
        }
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
      THEN("it should return 1 path with 3 nodes") { REQUIRE(solution == referenced_solution); }
    }
    GIVEN("Additional words that causes 2 paths to occur") {
      lexicon.insert("can");
      WHEN("Find Ladder is called") {
        auto solution = FindLadder("cat", "con", lexicon);
        std::sort(solution.begin(), solution.end());
        ListOfString referenced_solution = {{"cat", "can", "con"}, {"cat", "cot", "con"}};
        THEN("it should return 2 path with 3 nodes") { REQUIRE(solution == referenced_solution); }
      }
    }
  }
}

SCENARIO("Test 3: Correctness in populated dictionary to check whether it will ignore unrelated "
         "words") {
  GIVEN("A populated Dictionary") {
    std::unordered_set<std::string> lexicon;
    std::vector<std::string> relevant_data(
        {"cat", "cot", "cog", "bog", "bat", "con", "can", "cam"});
    lexicon.insert(relevant_data.begin(), relevant_data.end());
    std::vector<std::string> junk_data({"aa", "bb", "cc", "eeee", "zzzz", "aese", "fdae"});
    lexicon.insert(junk_data.begin(), junk_data.end());

    WHEN("Find Ladder is Called") {
      auto solution = FindLadder("cat", "con", lexicon);
      std::sort(solution.begin(), solution.end());
      ListOfString referenced_solution = {{"cat", "can", "con"}, {"cat", "cot", "con"}};
      THEN("it should return 2 path with 3 nodes") { REQUIRE(solution == referenced_solution); }
    }
  }
}

SCENARIO("Test 4: Correctness in More Branches of Paths") {
  GIVEN("A populated Dictionary") {
    std::unordered_set<std::string> lexicon;
    std::vector<std::string> data(
        {
          "work", "fork", "pork", "word", "worm", "worn", "form", "port",
          "wood", "bort", "wert", "foam", "peak", "pert", "pirn", "pert", "pood", "boat", "peat",
          "blat", "pian", "flam", "plod", "flam", "flay", "play", "plat", "plan", "plat", "plot",
          "play"
        });
    lexicon.insert(data.begin(), data.end());
      WHEN("Find Ladder is Called") {
        auto solution = FindLadder("work", "play", lexicon);
        std::sort(solution.begin(), solution.end());
        ListOfString referenced_solution = {
            {"work", "fork", "form", "foam", "flam", "flay", "play"},
            {"work", "pork" ,"port", "pert", "peat", "plat", "play"},
            {"work", "worm", "form", "foam", "flam", "flay", "play"}
        };
        THEN("it should return some path with 7 nodes") { REQUIRE(solution == referenced_solution);}
    }
  }
}
