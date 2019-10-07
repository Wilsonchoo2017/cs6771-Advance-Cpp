/*
 * Done by z5157656 Choo Yee Hang and z5161978 Matthew Chhoeu
 * This file contains the implementation for Generic Directed Weighted graph implementation.
 * The interface is in graph.h
 */
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

// ===================== Constructors ===================
template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator start,
                         typename std::vector<N>::const_iterator end) {
  for (auto it = start; it != end; ++it) {
    InsertNode(*it);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator start,
                         typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  for (auto it = start; it != end; ++it) {
    // Check if current node already exists in graph
    // Add node to graph
    InsertNode(std::get<0>(*it));
    InsertNode(std::get<1>(*it));

    // Check if current edge already exists in graph
    // Add edge to graph
    InsertEdge(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> list) {
  for (auto it = list.begin(); it != list.end(); ++it) {
    InsertNode(*it);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& g) {
  // Iterate through Map
  for (auto it = g.nodes.begin(); it != g.nodes.end(); ++it) {
    InsertNode(*(it->first));
  }
  // Copy key and values to new map
  for (auto node = g.nodes.begin(); node != g.nodes.end(); ++node) {
    for (auto edge = node->second.begin(); edge != node->second.end(); ++edge) {
      InsertEdge(*(node->first), *(edge->first.lock()), *(edge->second));
    }
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& g) : nodes(move(g.nodes)) {}

// ================= Operation ====================================
template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(const gdwg::Graph<N, E>& g) {
  Graph tmp{g};
  std::swap(tmp, *this);
  return *this;
}
template <typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N, E>::operator=(gdwg::Graph<N, E>&& g) {
  nodes = std::move(g.nodes);
  return *this;
}

// ================= Methods ======================================
template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N& val) noexcept {
  // Check if node already in graph
  if (IsNode(val)) {
    return false;
  }

  // Insert Node
  nodes.insert(std::make_pair(
      std::make_shared<N>(val),
      std::set<std::pair<std::weak_ptr<N>, std::unique_ptr<E>>, gdwg::Graph<N, E>::lex_compare>()));

  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N& src, const N& dst, const E& w) {
  // Check if both src and dest exist in graph
  if (!IsNode(src) || !IsNode(dst)) {
    // Exception thrown if either src and/or dest doesn't exist in graph
    throw(std::runtime_error(
        "Cannot call Graph::InsertEdge when either src or dst node does not exist"));
  }

  // Note: Following ensure only one outgoing edge for each node
  // Check if edge exists
  auto srcNode = nodes.find(std::make_shared<N>(src));

  if (srcNode->second.find(std::make_pair(std::make_shared<N>(dst), std::make_unique<E>(w))) !=
      srcNode->second.cend()) {
    return false;
  }

  // Create and insert new edge
  auto dstNode = nodes.find(std::make_shared<N>(dst));
  srcNode->second.emplace(std::make_pair(std::weak_ptr<N>(dstNode->first), std::make_unique<E>(w)));

  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N& n) noexcept {
  // Check if n is a valid node
  if (!IsNode(n)) {
    return false;
  }

  // Delete incoming edges to n
  for (auto it = cbegin(); it != cend(); ++it) {
    if (std::get<1>(*it) == n) {
      erase(it);
    }
  }

  // Delete node
  nodes.erase(std::make_shared<N>(n));

  return true;
}

template <typename N, typename E>
void gdwg::Graph<N, E>::Clear() noexcept {
  nodes.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N& val) const noexcept {
  // Search map for node
  auto it = std::find_if(nodes.begin(), nodes.end(),
                         [&](auto const& ptr) { return *(ptr.first) == val; });
  return it != nodes.end() ? true : false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N& src, const N& dst) const {
  // Check if both src and dest exist in graph
  if (!IsNode(src) || !IsNode(dst)) {
    // Exception thrown if either src and/or dest doesn't exist in graph
    throw(std::runtime_error(
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph"));
  }

  // Else check if edge exists
  auto srcNode = nodes.find(std::make_shared<N>(src));

  // Iterate through set checking if an edge to dst exists
  for (auto it = srcNode->second.begin(); it != srcNode->second.end(); ++it) {
    if (*(it->first.lock()) == dst) {
      return true;
    }
  }

  return false;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const noexcept {
  std::vector<N> newV;
  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    newV.push_back(*(it->first));
  }

  return newV;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N& src) const {
  if (!IsNode(src)) {
    throw(std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph"));
  }

  std::vector<N> newV;
  // Get the srcNode from map
  auto srcNode = std::find_if(nodes.begin(), nodes.end(),
                              [&](auto const& ptr) { return *(ptr.first) == src; });

  // Iterate through the set (edges)
  for (auto it = srcNode->second.begin(); it != srcNode->second.end(); ++it) {
    if (std::find(newV.begin(), newV.end(), *(it->first.lock())) == newV.end()) {
      // only add if not in vector already
      newV.push_back(*(it->first.lock()));
    }
  }

  return newV;
}

template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N& src, const N& dst) const {
  // Check if both src and dest exist in graph
  if (!IsNode(src) || !IsNode(dst)) {
    // Exception thrown if either src and/or dest doesn't exist in graph
    throw(std::runtime_error(
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph"));
  }

  std::vector<E> newV;
  // Get srcNode
  auto srcNode = nodes.find(std::make_shared<N>(src));
  // Iterate through edges
  for (auto it = srcNode->second.begin(); it != srcNode->second.end(); ++it) {
    if (*(it->first.lock()) == dst) {
      // Add weight if edge to dst exists
      newV.push_back(*(it->second));
    }
  }
  return newV;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N& oldData, const N& newData) {
  if (!IsNode(oldData)) {
    // Exception thrown if either src and/or dest doesn't exist in graph
    throw(std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist"));
  }
  if (IsNode(newData)) {
    // newData already in graph, replace fails
    return false;
  }

  // Add node to graph
  InsertNode(newData);

  // Replace the oldData with newData
  MergeReplace(oldData, newData);

  return true;
}

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N& oldData, const N& newData) {
  if (!IsNode(oldData) || !IsNode(newData)) {
    // Exception thrown if either src and/or dest doesn't exist in graph
    throw(std::runtime_error(
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph"));
  }

  // if there's a dup newData's  takes priority and replaces it.
  // Get ALl outgoing links of old data
  auto old_it = nodes.find(std::make_shared<N>(oldData));
  auto new_it = nodes.find(std::make_shared<N>(newData));
  new_it->second.merge(old_it->second);

  // get ALL ingoing links of old data
  std::set<std::pair<std::weak_ptr<N>, std::unique_ptr<E>>> ingoing_nodes{};
  for (auto outer_it = nodes.begin(); outer_it != nodes.end(); ++outer_it) {
    if (*outer_it->first == oldData)
      continue;  // skip if oldData
    for (auto inner_it = outer_it->second.begin(); inner_it != outer_it->second.end(); ++inner_it) {
      if (*inner_it->first.lock() == oldData) {
        InsertEdge(*outer_it->first, newData, *inner_it->second.get());
      }
    }
  }
  DeleteNode(oldData);
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::find(const N& src, const N& dst, const E& w) const noexcept {
  return std::find(cbegin(), cend(), std::tie(src, dst, w));
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N& src, const N& dst, const E& w) noexcept {
  auto it = std::find(cbegin(), cend(), std::tie(src, dst, w));
  // Check if the edge is valid
  if (it == cend()) {
    return false;
  }
  // Erase edge
  erase(it);

  return true;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::erase(const_iterator it) noexcept {
  // Check if it is at end
  if (it == cend()) {
    return cend();
  }
  // Check if it points to something valid
  auto temp = find(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
  if (it != temp) {
    return cend();
  }

  // Get values of next edge
  bool flag = true;
  N src;
  N dst;
  E w;
  if (++temp != cend()) {
    // One node left need to return cend()
    flag = false;
    src = std::get<0>(*temp);
    dst = std::get<1>(*temp);
    w = std::get<2>(*temp);
  }

  // Get edge location
  auto node = nodes.find(std::make_shared<N>(std::get<0>(*it)));
  node->second.erase(std::make_pair(std::weak_ptr<N>(std::make_shared<N>(std::get<1>(*it))),
                                    std::make_unique<E>(std::get<2>(*it))));
  return flag ? cend() : find(src, dst, w);
}

// =============== Iterator Methods =====================

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() const {
  auto first_it = std::find_if(nodes.cbegin(), nodes.cend(),
                               [](const auto& ptr) { return !ptr.second.empty(); });
  auto last_it = nodes.cbegin();
  for (auto it = nodes.cbegin(); it != nodes.cend(); ++it) {
    if (!it->second.empty()) {
      last_it = it;
    }
  }

  if (first_it != nodes.cend()) {
    return const_iterator{first_it, first_it, nodes.cend(), first_it->second.cbegin(), last_it};
  }
  // graph is empty
  return cend();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() const {
  auto first_it = std::find_if(nodes.cbegin(), nodes.cend(),
                               [](const auto& ptr) { return !ptr.second.empty(); });
  auto last_it = nodes.cend();
  for (auto it = nodes.cbegin(); it != nodes.cend(); ++it) {
    if (!it->second.empty()) {
      last_it = it;
    }
  }
  if (first_it != nodes.cend()) {
    return const_iterator{nodes.cend(), first_it, nodes.cend(), {}, last_it};
  }
  // graph is empty
  return const_iterator{nodes.cend(), nodes.cbegin(), nodes.cend(), {}, last_it};
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator++() {
  if (outer_ == sent_end_) {
    return *this;
  }
  ++inner_;
  if (inner_ == outer_->second.cend()) {
    do {
      ++outer_;
    } while (outer_ != sent_end_ && outer_->second.cbegin() == outer_->second.cend());
    if (outer_ != sent_end_) {
      inner_ = outer_->second.cbegin();
    }
  }
  return *this;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator& gdwg::Graph<N, E>::const_iterator::operator--() {
  if (outer_ == sent_end_) {
    // case when we are at dummy variable
    if (last_inner_ == sent_end_)
      return *this;

    // set inner as our last it from last_inner
    inner_ = last_inner_->second.end();
    --inner_;
    // reverse our outer
    outer_ = last_inner_;

    return *this;
  }
  if (inner_ == outer_->second.cbegin()) {
    if (outer_ == sent_start_)
      return *this;
    do {
      --outer_;
    } while (outer_ != sent_start_ && outer_->second.cbegin() == outer_->second.cend());
    if (outer_->second.cbegin() != outer_->second.cend()) {
      inner_ = outer_->second.cend();
      --inner_;
    }
  } else {
    --inner_;
  }
  return *this;
}
