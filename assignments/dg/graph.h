/*
 * Done by z5157656 Choo Yee Hang and z5161978 Matthew Chhoeu
 * This file contains the API for Generic Directed Weighted graph implementation.
 * The implementation is in graph.tpp. It uses only STL libraries.
 * More details of each API is commented above each API.
 * A Custom Iterator is used to represent our Edges.
 */

#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

namespace gdwg {

template <typename N, typename E>
class Graph {
 public:
  struct CompareByValue {
    bool operator()(const std::shared_ptr<N>& left, const std::shared_ptr<N>& right) const {
      return *left < *right;
    }
  };

  struct lex_compare {
    typedef std::pair<std::weak_ptr<N>, std::unique_ptr<E>> p;
    bool operator()(const p& lhs, const p& rhs) const {
      auto lptr = lhs.first.lock();
      auto rptr = rhs.first.lock();
      if (!rptr)
        return false;  // nothing after expired pointer
      if (!lptr)
        return true;  // every not expired after expired pointer
      if (*lptr == *rptr) {
        return *(lhs.second) < *(rhs.second);
      }
      return *lptr < *rptr;
    }
  };
  class const_iterator {
    /*
     * This is a custom itertor class for Graph class to represent edges.
     * all iterators are invalidated after any modification of the graph
     * thus all operation should be a const
     * The ordering of the edges is sorted by
     * 1) Src node (increasing order); then 2) Dst node (increasing order); then 3) Edge weight
     * (increasing order) we used the code above, lexcompare and comparebyValue.
     */
   public:
    // Iterator Traits
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;
    using pointer = std::tuple<const N*, const N*, const E*>;
    using difference_type = int;

    reference operator*() const {
      return std::tie(*outer_->first, *inner_->first.lock(), *inner_->second);
    }
    pointer operator->() const { return &(operator*()); }
    const_iterator& operator++();
    const_iterator operator++(int) {
      auto copy{*this};
      ++(*this);
      return copy;
    }
    const_iterator& operator--();
    const_iterator operator--(int) {
      auto copy{*this};
      --(*this);
      return copy;
    }

    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
      return lhs.outer_ == rhs.outer_ && (lhs.outer_ == lhs.sent_end_ || lhs.inner_ == rhs.inner_);
    }

    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
      return !(lhs == rhs);
    }

   private:  // const_iterator
    /* outer keeps track of the pos of the outer layer, it->first
     * inner keeps track of the post of the inner layer, it->second
     * sent_start keeps track of the first item that is not empty (!inner.empty())
     * sent_end keeps track of the end of the graph.
     * last_inner track of the last item that is not empty (!outer.empty())
     */
    typename std::map<std::shared_ptr<N>,
                      std::set<std::pair<std::weak_ptr<N>, std::unique_ptr<E>>, lex_compare>,
                      CompareByValue>::const_iterator outer_;
    const typename std::map<std::shared_ptr<N>,
                            std::set<std::pair<std::weak_ptr<N>, std::unique_ptr<E>>, lex_compare>,
                            CompareByValue>::const_iterator sent_start_;
    const typename std::map<std::shared_ptr<N>,
                            std::set<std::pair<std::weak_ptr<N>, std::unique_ptr<E>>, lex_compare>,
                            CompareByValue>::const_iterator sent_end_;
    typename std::set<std::pair<std::weak_ptr<N>, std::unique_ptr<E>>, lex_compare>::const_iterator
        inner_;
    typename std::map<std::shared_ptr<N>,
                      std::set<std::pair<std::weak_ptr<N>, std::unique_ptr<E>>, lex_compare>,
                      CompareByValue>::const_iterator last_inner_;

    friend class Graph;
    const_iterator(const decltype(outer_)& outer,
                   const decltype(sent_start_)& start,
                   const decltype(sent_end_)& end,
                   const decltype(inner_)& inner,
                   const decltype(last_inner_)& last_inner)
      : outer_{outer}, sent_start_{start}, sent_end_(end), inner_{inner}, last_inner_{last_inner} {}
  };
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  const_iterator begin() const { return cbegin(); }
  const_iterator cbegin() const;
  const_iterator end() const { return cend(); }
  const_iterator cend() const;

  const_reverse_iterator rbegin() const { return crbegin(); }
  const_reverse_iterator rend() const { return crend(); }
  const_reverse_iterator crbegin() const { return const_reverse_iterator{cend()}; }
  const_reverse_iterator crend() const { return const_reverse_iterator{cbegin()}; }

  // Start of Graph Definition
  // Constructors For Graph Class
  Graph() = default;
  Graph(typename std::vector<N>::const_iterator, typename std::vector<N>::const_iterator);
  Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator,
        typename std::vector<std::tuple<N, N, E>>::const_iterator);
  Graph(std::initializer_list<N>);
  // https://webcms3.cse.unsw.edu.au/COMP6771/19T2/forums/2733258#2733272
  // Says that expicit worning from linter can be ignored
  Graph(const gdwg::Graph<N, E>&);
  Graph(gdwg::Graph<N, E>&&);
  ~Graph() = default;

  // Operations For Graph
  Graph& operator=(const Graph& g);
  Graph& operator=(Graph&& g);

  // Methods For Graph Class
  /*
   * InsertNode: Adds a new node with value given into the graph.
   * Returns false with graph unchanged if node is already in graph
   */
  bool InsertNode(const N& val) noexcept;
  /*
   * InsertEdge: Adds a new edge src -> dst with weight w.
   * Return true if edge is sucessfully added. Else return false with graph unchanged
   * Can have multiple srcNode pointing at dstNode as long as their weights are different
   * Throws an runtime exception if either src or dst not cannot be found in the graph
   */
  bool InsertEdge(const N& src, const N& dst, const E& w);
  /*
   * DeleteNode: Deletes a given node and all its ingoing and outgoing edges
   * Does nothing if the node doesn't exist in the graph
   * Return True if node sucessfully delete. Else return false with graph unchanged
   */
  bool DeleteNode(const N&) noexcept;
  /*
   * Replace: Replaces Original data, old data with newData.
   * This function calls InsertNode, MergeReplace for it's implementation:w
   * returns true if successfully replace original data else return false if
   * node contains value newData already exist Throws an runtime exception if
   * no node that contains value oldData can be found
   */
  bool Replace(const N& oldData, const N& newData);
  /*
   * MergeReplace: All instances of node oldData in the graph are replaced with instances of
   * newData. After completing, every incoming and outgoing edge of oldData becomes an
   * incoming/ougoing edge of newData, except that duplicate edges must be removed. This function
   * calls DeleteNode  for it's implemntation Throws an runtime exception if either node cannot be
   * found in the graph
   */
  void MergeReplace(const N& oldData, const N& newData);
  /*
   * IsConnected: Checks if edge from src to dst exist
   * returns true if so, else false
   * Throws an runtime exception if either src or dst is not in the graph
   */
  bool IsConnected(const N& src, const N& dst) const;

  /*
   * Clear: Remove all nodes and edges from the graph.
   */
  void Clear() noexcept;
  /*
   * IsNode: checks if a node with a value exist in the graph
   * returns true if so else false
   */
  bool IsNode(const N& val) const noexcept;
  std::vector<N> GetNodes() const noexcept;
  std::vector<N> GetConnected(const N& src) const;
  std::vector<E> GetWeights(const N& src, const N& dst) const;
  /*
   * find: finds the given src -> dst with weight w.
   * returns true if found, else false
   */
  const_iterator find(const N&, const N&, const E&) const noexcept;
  /*
   * erase: deletes and edge from src -> dst with weight w,
   * returns true if sucessful else false if edge cant be found in the graph
   * with graph unchanged
   */
  bool erase(const N& src, const N& dst, const E& w) noexcept;
  /*
   * erase: same as above, but deletes the node at pos it.
   */
  const_iterator erase(const_iterator it) noexcept;

 private:  // Graph
  typename std::map<std::shared_ptr<N>,
                    std::set<std::pair<std::weak_ptr<N>, std::unique_ptr<E>>, lex_compare>,
                    CompareByValue>
      nodes;

  // =============================== Definition of Friend Functions ===============================

  // == operator deep compares the items
  friend bool operator==(const Graph<N, E>& lhs, const Graph<N, E>& rhs) {
    std::vector<N> v1 = lhs.GetNodes();
    std::vector<N> v2 = rhs.GetNodes();
    return std::equal(lhs.begin(), lhs.end(), rhs.begin()) && v1 == v2;
  }

  friend bool operator!=(const Graph<N, E>& lhs, const Graph<N, E>& rhs) { return !(lhs == rhs); }

  friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
    for (auto outer_it = g.nodes.begin(); outer_it != g.nodes.end(); ++outer_it) {
      os << *outer_it->first << " (\n";
      for (auto inner_it = outer_it->second.begin(); inner_it != outer_it->second.end();
           ++inner_it) {
        os << "  " << *inner_it->first.lock() << " | " << *inner_it->second.get() << "\n";
      }
      os << ")\n";
    }
    return os;
  }
};

}  // namespace gdwg

#include "assignments/dg/graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
