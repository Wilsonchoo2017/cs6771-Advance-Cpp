#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"

void PrintResult(ListOfString&);

int main() {
  auto lexicon = GetLexicon("assignments/wl/words.txt");  // lexicon = unordered set

  // Prompt user for start and end word. No Error Handling Required!
  std::string start_word;
  // https://stackoverflow.com/questions/9243144/how-to-exit-program-on-press-of-return-key
  std::cout << "Enter start word (RETURN to quit): ";
  std::getline(std::cin, start_word);
  if (start_word.empty()) {
    return 0;
  }
  std::string end_word;
  std::cout << "Enter destination word: ";
  std::getline(std::cin, end_word);
  if (end_word.empty()) {
    return 0;
  }

  // Convert all input to lower case
  //  std::transform(start_word.begin(), start_word.end(), start_word.begin(), ::tolower);
  //  std::transform(end_word.begin(), end_word.end(), end_word.begin(), ::tolower);

  // Check that start and end word exist in lexicon
  if (lexicon.find(start_word) == lexicon.end() || lexicon.find(end_word) == lexicon.end()) {
    // No such words, thus print result
    std::cout << "No ladder found.\n";
    return 0;
  }

  ListOfString solution = FindLadder(start_word, end_word, lexicon);

  PrintResult(solution);
  return 0;
}

void PrintResult(ListOfString& list) {
  if (list.empty()) {
    std::cout << "No ladder found.\n";
  } else {
    std::cout << "Found ladder: ";
    // sort the list
    std::sort(list.begin(), list.end());
    // print out list
    // https://stackoverflow.com/questions/21714512/how-to-print-vectorvectorstring-in-c
    for (auto const& string_vec : list) {
      for (auto const& word : string_vec) {
        std::cout << word << " ";
      }
      std::cout << "\n";
    }
  }
}
