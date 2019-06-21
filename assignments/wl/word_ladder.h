#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using ListOfString = std::vector<std::vector<std::string>>;

struct WordNode {
  int hopped;
  std::string word;
  std::vector<WordNode*> previous_word_nodes;
  WordNode(int hopped, std::string word) : hopped(hopped), word(std::move(word)) {}  // constructor
};

ListOfString
FindLadder(std::string const&, std::string const&, std::unordered_set<std::string> const&);
void GetAllLadder(ListOfString&, std::deque<std::string>&, WordNode*);

#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_
