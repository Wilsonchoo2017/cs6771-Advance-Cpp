#include "assignments/wl/word_ladder.h"

#include <algorithm>
#include <deque>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
 * Precondition: start and end word exists in lexicon.
 * PostCondition: return empty vector if no ladder is found. Otherwise return an unsorted ladder
 * Uses BFS to find the relationship of words. Once end_word is found, we call FindAllPaths to
 * construct our solution and return it.
 */
ListOfString FindLadder(std::string const& start_word,
                        std::string const& end_word,
                        std::unordered_set<std::string> const& dict) {
  ListOfString solution_ladder;

  // Have an early exit when start and end word are identical
  if (start_word == end_word) {
    solution_ladder.push_back(std::vector<std::string>{start_word, end_word});
    return solution_ladder;
  }

  // Initialise queue and visited
  std::queue<std::string> queue;
  queue.push(start_word);
  std::unordered_map<std::string, WordNode*> visited;
  visited.emplace(start_word, new WordNode(0, start_word));

  while (!queue.empty()) {
    // Dequeue
    auto curr_word = queue.front();
    queue.pop();

    // Get Details of curr_word
    WordNode* curr_word_node = visited.find(curr_word)->second;
    auto curr_level = curr_word_node->hopped;

    if (curr_word == end_word) {
      // Here we are guaranteed to have the next level if we reached the end node.
      // Thus we can now find all the paths that is connected to the start word.
      std::deque<std::string> subqueue;
      subqueue.emplace_back(end_word);
      GetAllLadder(solution_ladder, subqueue, curr_word_node);
      return solution_ladder;
    }

    // Find all possible words
    std::string og_curr_word = curr_word;
    for (auto& letter_it : curr_word) {
      auto og_letter_it = letter_it;
      // setup variables for loop
      for (auto char_it = 'a'; char_it <= 'z'; char_it++) {
        if (char_it == og_letter_it)
          continue;
        letter_it = char_it;
        if (dict.find(curr_word) != dict.end()) {
          // word found in the dictionary
          auto node_it = visited.find(curr_word);
          if (node_it != visited.end()) {
            // String has been visited
            WordNode* node = node_it->second;
            if (node->hopped == curr_level + 1) {
              // Valid paths we add the parent node into this node previous
              node->previous_word_nodes.push_back(curr_word_node);
            }  // else skip as not a valid path
          } else {
            // string has not been visited, we creat and add it into our visited, enqueue it and
            // add it to the parent node  into this node previous
            WordNode* new_word_node = new WordNode(curr_level + 1, curr_word);
            new_word_node->previous_word_nodes.emplace_back(curr_word_node);
            visited.emplace(curr_word, new_word_node);
            queue.push(curr_word);
          }
        }  // else skip if such words dont exist
      }
      curr_word = og_curr_word;  // revert changes
    }
  }
  return solution_ladder;  // Empty ListOfString
}

// Uses DFS to find all paths to origin nodes.
// https://www.geeksforgeeks.org/depth-first-search-or-dfs-for-a-graph/
void GetAllLadder(ListOfString& solution_ladder,
                  std::deque<std::string>& subqueue,
                  WordNode* const curr_node) {
  std::vector<WordNode*> previous_words = curr_node->previous_word_nodes;

  // The Recursion
  if (!previous_words.empty()) {
    for (const auto& word_node : previous_words) {
      subqueue.emplace_front(word_node->word);  // insert current word into beginning of sublist
      GetAllLadder(solution_ladder, subqueue, word_node);  // recursively call function
      subqueue.pop_front();  // Revert to allow the next word construct a path
    }
  } else {
    // Base Case
    std::vector<std::string> sublist(subqueue.begin(), subqueue.end());
    solution_ladder.emplace_back(sublist);
    return;
  }
}
