#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <string>
#include <deque>

using ListOfString = std::vector<std::vector<std::string>>;

struct WordNode
{
  int hopped;
  std::string word;
  // Vector instead of wordNode only as we refer more than 1 word
  std::vector<WordNode *> previous_word_nodes;
  WordNode (int hopped, std::string word): hopped(hopped), word(std::move(word)) {};// constructor
};

ListOfString FindLadder(std::string const &, std::string const &,
    std::unordered_set<std::string> &);
void GetAllLadder(ListOfString &, std::deque<std::string> &, WordNode *);

#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_
