/*
 * Done by z5157656 Choo Yee Hang and z5161978 Matthew Chhoeu
 * This file contains the testing for Generic Directed Weighted Graph implmentation
 * == Explanation and rational of testing ==
 * If You want a good formatting visit:
https://www.notion.so/Testing-f44cbde91ffe43f2a40d3f4c3fdbd5f8 # Testing Notes

### Introduction

There is a circular dependency within the testing/verification of functions.
To verify the private contents of an instance of a graph certain member functions are required to
obtain this information. But to verify these member functions we have to valid constructor. Thus, a
circular dependency.

Therefore, an assumption must be made during testing, that either the methods or constructor are
valid. Henceforth, presuming trivial getters removes this dependency. The follow methods are assumed
correct for the validation of constructors.

- `bool IsNode(const N& val);` - see of the nodes have been added properly
- `bool IsConnected(const N& src, const N& dst);` - see if edges have been added properly
- `std::vector<E> GetWeights(const N& src, const N& dst)` - see if edge weights have been added
properly

### Const Graph Testing

The graph is required to handle const graphs.
A const object can execute methods that do not change the state of the object (i.e. edit/change
member variables). There are few functions to look out for:
- Iterator methods
- `bool IsNode(const N& val);`
- `bool IsConnected(const N& src, const N& dst);`
- `std::vector<N> GetNodes()`
- `std::vector<N> GetConnected(const N& src)`
- `std::vector<E> GetWeights(const N& src, const N& dst)`
- `const_iterator find(const N&, const N&, const E&);`

# Testing Order

### 1. Assumptions

Assumptions that were made previously in intorduction

### 2. Default Constructor

All the other constructors relies on the InsertNode and InsertEdge methods,
excluding the default constructor which just initializes the map for the nodes and sets size to 0.
Testing the default constructor would be done by checking if the map is empty e.g. Using GetNodes to
see if the map h as no nodes; map is empty. However, this is a trivial matter, as such it can be
presumed that the default constructor functions correctly.

### 3. Inserting Methods

As the other constructors rely on the insertion methods. This needs to be tested next. These
 methods are tested by the methods that were presumed correct. I.e.
InsertNode - tested by IsNode
InsertEdge - tested by IsConnected and GetWeights

bool InsertNode(const N& val)
  - Case 1: The new node trying to be added exists in the graph. Return false.
  - Case 2: The new node trying to be added does not exist in the graph. Return true.
bool InsertEdge(const N& src, const N& dst, const E& w)
**Condition**: If either src or dst not cannot be found in the graph
**Throw**: std::runtime_error
**With string**: "Cannot call Graph::InsertEdge when either src or dst node does not exist"
  - Case 1: New edge trying to be added exists in the graph. Return false. Note: valid inputs
  - Case 2: New edge trying to be added does not exist in the graph. Return true. Note: valid inputs
  - Case 3: New edge trying to be added does not have a valid src. `std::runtime_error` is thrown
  - Case 4: New edge trying to be added does not have a valid dest. `std::runtime_error` is thrown

### 4. Constructors

Now that Inserting Methods are validated the rest of the constructors can be tested.

gdwg::Graph<N, E>(
  std::vector<N>::const_iterator,
  std::vector<N>::const_iterator)
  - Case 1: Creates a graph with nodes from the iterator
gdwg::Graph<N, E>(
  std::vector<std::tuple<N, N, E>>::const_iterator,
  std::vector<std::tuple<N, N, E>>::const_iterator)
  - Case 1: Creates a graph with nodes and edges defined from the tuple iterator
gdwg::Graph<N, E>(std::initializer_list<N>)
  - Case 1: Creates a graph with nodes from the list passed
gdwg::Graph<N, E>(const gdwg::Graph<N, E>&)
  - Case 1: Creates a deep copy of the graph passed
gdwg::Graph<N, E>(gdwg::Graph<N, E>&&)
  - Case 1: Moves all data from passed graph to new graph. Passed graph is now empty.
  Note: To check if the graph is empty we need the getter methods so this constructor is tested
  later

### 5. Getter Methods

The getter methods are next. Since these methods could not be tested until a graph had nodes and
edges it is tested next. These methods can be useful for debugging.

std::vector<N> GetNodes()
  - Case 1: Graph empty. Return empty vector.
  - Case 2: Graph not empty. Return vector of nodes in increasing order.
std::vector<N> GetConnected(const N& src)
**Condition**: If src is not in the graph
**Throw**: std::out_of_range
**With string**: "Cannot call Graph::GetConnected if src doesn't exist in the graph"
  - Case 1: Node exists in graph but has no outgoing edges. Return empty vector.
  - Case 2: Node exists in graph and has outgoing edges. Return vector of nodes in increasing order.
  - Case 3: Node does not exist in graph. `std::out_of_range` is thrown
  - Case 4: If there are multiple outgoing edges to one particular node. Dst node only appears once.

### 6. Operator Overload <<

Since graphs can be made. We can test operator<< as it can prove useful for debugging purposes as
well as it is independent to all other methods.
The operator is tested using auto string stream and comparing to string.
Need to test that the string comes in increasing order based from the node, edge and then weight

### 7. Clear

Since the getter methods are tested now it is possible to check if the graph is empty.
 Thus, clear as well as the move constructor can be tested
void Clear()
  - Case 1: Graph becomes empty. (i.e. no edges and nodes)

### 8. Iterator Operators

The next thing need to test is the validity of the iterator and its methods. Furthermore functions
are dependent on these iterator methods working i.e. find

The Scenario is broken down into 5 pieces.

1. cbegin and cend used to test ++ operator
2. cbegin and cend along side with ++ operator to test `--` operator
3. crbegin and crend used to test ++ operator
4. crbegin and crend along side with ++ operator to test `--` operator
5. Checks for ordering of iterator

For 1~4, std::get<x> is used to compare the internal value. The ordering of testing here is
important because `++` and `--` must work with nornal begin() and end() before being functional in
the rbegin() and rend(). For 2, 4, logically it's okay to declare cend() and do `--` operation from
there. But we believe that this is error prone and hard to debug because there would be assumption
of `--` working straight out of the box. Thus we used `++` to reach the end and `--` back to the
beginning.

Our Test case are very extensive and have the same 4 * 4 edge cases for each operator to ensure it
works with these edge cases. Finally for checking our ordering, we declare our nodes and edges in an
reverse order to ensure it gets sorted

### 9. Find

As the iterator is now valid testing find is necessary as following methods are dependent on its
validity
const_iterator find(const N&, const N&, const E&)
  - Case 1: the desired edge not in the graph, returns cend()
  - Case 2: the desired edge is in the graph, returns iterator to that edge

### 10. Erase

Dependent on the find method
bool erase(const N& src, const N& dst, const E& w)
  - Case 1: the desired edge not in the graph, dont erase, returns false
  - Case 2: the desired edge is in the graph, erase,returns true
const_iterator erase(const_iterator it)
  - Case 1: the desired edge not in the graph, dont erase, returns cend()
  - Case 2: the desired edge is in the graph, erase, returns iterator to next edge
  - Case 3: the passed it is cend(), returns cend()

### 11. DeleteNode

Dependent on the erase method
bool DeleteNode(const N&)
  - Case 1: Delete a node that does not exist in the graph. Returns false.
  - Case 2: Delete a node that exists in the graph. Returns true.
  - Case 3: Delete a node that exists in the graph with outgoing edges. Returns true.
  - Case 4: Delete a node that exists in the graph with incoming edges. Returns true.

### 12. Replace and Merge Replace

Dependent on the deleteNode method
bool Replace(const N& oldData, const N& newData)
**Condition**: If no node that contains value oldData can be found
**Throw**: std::runtime_error
**With string**: "Cannot call Graph::Replace on a node that doesn't exist"
  - Case 1: If old data is not in the graph, exception thrown
  - Case 2: If newData is in the graph, replace fails, returns false
  - Case 3: If newData is not in the graph, replace occurs, returns true
void MergeReplace(const N& oldData, const N& newData)
**Condition**: If either node cannot be found in the graph
**Throw**: std::runtime_error
**With string**: "Cannot call Graph::MergeReplace on old or new data if they don't exist in the
graph"
  - Case 1: If old data is not in the graph, exception thrown
  - Case 2: If newData is not in the graph, exception thrown
  - Case 3: If oldData and newData is in the graph, merge replace occurs

### 13. Remaining Operators

The assignment and equals operators are tested last as nothing is dependent on these working nor was
it needed for the testing and debugging process.
bool operator==(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&)
  - Case 1: LHS and RHS have the same nodes and edges. Return true.
  - Case 2: LHS and RHS do not have the same nodes and edges. Return false.
bool operator!=(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&)
  - Case 1: LHS and RHS have the same nodes and edges. Return false.
  - Case 2: LHS and RHS do not have the same nodes and edges. Return true.
gdwg::Graph<N, E>& operator=(const gdwg::Graph<N, E>&)
  - Case 1: Creates a deep copy of the graph equated to
gdwg::Graph<N, E>& operator=(gdwg::Graph<N, E>&&)
  - Case 1: Moves all data from assigned graph (RHS) to new graph (LHS). Passed graph is now empty.

*/

#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "assignments/dg/graph.h"
#include "catch.h"

// ==== Inserting Methods ====
SCENARIO("Testing InsertNode") {
  GIVEN("An empty graph") {
    gdwg::Graph<std::string, int> g1;
    std::string s1{"Hello"};
    WHEN("Inserting node that does not exist in graph") {
      THEN("Method returns true") {
        REQUIRE(g1.InsertNode(s1));
        REQUIRE(g1.IsNode(s1));
      }
    }
  }

  GIVEN("A graph") {
    gdwg::Graph<std::string, int> g1;
    std::string s1{"Hello"};
    g1.InsertNode(s1);
    WHEN("Inserting node that does exist in graph") {
      THEN("Method returns false") { REQUIRE(!g1.InsertNode(s1)); }
    }
  }
}

SCENARIO("Testing InsertEdge") {
  GIVEN("A graph with nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("hello");
    g.InsertNode("how");
    WHEN("Trying to insert an edge with invalid src node") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(g.InsertEdge("bye", "how", 5), std::runtime_error);
        REQUIRE_THROWS_WITH(
            g.InsertEdge("bye", "how", 5),
            "Cannot call Graph::InsertEdge when either src or dst node does not exist");
      }
    }
    WHEN("Trying to insert an edge with invalid dst node") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(g.InsertEdge("hello", "cow", 5), std::runtime_error);
        REQUIRE_THROWS_WITH(
            g.InsertEdge("hello", "cow", 5),
            "Cannot call Graph::InsertEdge when either src or dst node does not exist");
      }
    }
  }
  GIVEN("A graph with nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("hello");
    g.InsertNode("how");
    WHEN("Trying to insert an edge with valid parameters that does not exist in the graph") {
      THEN("Edge is added and method returns true") {
        REQUIRE(g.InsertEdge("hello", "how", 5));
        REQUIRE(g.IsConnected("hello", "how"));
        auto v1 = g.GetWeights("hello", "how");
        REQUIRE(v1[0] == 5);
      }
    }
  }
  GIVEN("A graph with nodes") {
    gdwg::Graph<std::string, int> g;
    g.InsertNode("hello");
    g.InsertNode("how");
    g.InsertEdge("hello", "how", 5);
    WHEN("Trying to insert an edge with valid parameters that does exist in the graph") {
      THEN("Edge is not and method returns false") { REQUIRE(!g.InsertEdge("hello", "how", 5)); }
    }
  }
}

// ==== Constructors =====
SCENARIO("Testing DG constructor from iterator") {
  GIVEN("A vector of strings") {
    std::vector<std::string> v1{"Hello", "Goodbye"};
    WHEN("Constructing DG from vector") {
      gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
      THEN("All strings in vector are valid nodes in graph") {
        for (int i = 0; i < static_cast<int>(v1.size()); ++i) {
          REQUIRE(g1.IsNode(v1[i]));
        }
      }
    }
  }
}

SCENARIO("Testing DG constructor from iterator of tuples") {
  GIVEN("A vector of tuples of <string, string, int>") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    WHEN("Constructing DG from vector") {
      gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};

      THEN("All strings in vector are valid nodes in graph") {
        for (int i = 0; i < static_cast<int>(v1.size()); ++i) {
          REQUIRE(g1.IsNode(std::get<0>(v1[i])));
          REQUIRE(g1.IsNode(std::get<1>(v1[i])));
        }
      }

      AND_THEN("Hello and how are connected by a weight of 4") {
        REQUIRE(g1.IsConnected(s1, s2));
        auto v2 = g1.GetWeights(s1, s2);
        REQUIRE(v2[0] == 4);
      }
      AND_THEN("how and cow are connected by a weight of 7") {
        REQUIRE(g1.IsConnected(s2, s3));
        auto v2 = g1.GetWeights(s2, s3);
        REQUIRE(v2[0] == 7);
      }
    }
  }
}

SCENARIO("Testing DG constructor from initializer_list") {
  GIVEN("An initializer_list") {
    WHEN("Constructing DG from initializer_list") {
      std::vector<std::string> v{"Hello", "how", "are", "you"};
      gdwg::Graph<std::string, int> g{"Hello", "how", "are", "you"};
      THEN("All nodes should be added to the graph from the list") {
        for (auto it = v.begin(); it != v.end(); ++it) {
          REQUIRE(g.IsNode(*it));
        }
      }
    }
  }
}

SCENARIO("Testing Copy Constructor") {
  GIVEN("A constructed graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Constructing DG from another DG") {
      gdwg::Graph<std::string, int> gCopy{g};
      THEN("All strings in vector are valid nodes in graph") {
        for (int i = 0; i < static_cast<int>(v1.size()); ++i) {
          REQUIRE(gCopy.IsNode(std::get<0>(v1[i])));
          REQUIRE(gCopy.IsNode(std::get<1>(v1[i])));
        }
      }
      AND_THEN("Hello and how are connected by a weight of 4") {
        REQUIRE(gCopy.IsConnected(s1, s2));
        auto v2 = gCopy.GetWeights(s1, s2);
        REQUIRE(v2[0] == 4);
      }
      AND_THEN("how and cow are connected by a weight of 7") {
        REQUIRE(gCopy.IsConnected(s2, s3));
        auto v2 = gCopy.GetWeights(s2, s3);
        REQUIRE(v2[0] == 7);
      }
    }
  }
}

// ==== Getter Methods =====
SCENARIO("Testing GetNodes") {
  GIVEN("An Empty Graph") {
    gdwg::Graph<std::string, int> g1;
    WHEN("Calling GetNodes") {
      auto nodes = g1.GetNodes();
      THEN("Empty node vector is return") { REQUIRE(nodes.size() == 0); }
    }
  }
  GIVEN("A Graph with nodes") {
    std::vector<std::string> v1{"Maths", "Hello", "Army"};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    WHEN("Calling GetNodes") {
      auto nodes = g1.GetNodes();
      THEN("Node vector is return in ascending order") {
        std::sort(v1.begin(), v1.end());
        for (int i = 0; i < 3; ++i) {
          REQUIRE(nodes[i] == v1[i]);
        }
      }
    }
  }
  GIVEN("A const Empty Graph") {
    const gdwg::Graph<std::string, int> g1;
    WHEN("Calling GetNodes") {
      auto nodes = g1.GetNodes();
      THEN("Empty node vector is return") { REQUIRE(nodes.size() == 0); }
    }
  }
  GIVEN("A const Graph with nodes") {
    std::vector<std::string> v1{"Maths", "Hello", "Army"};
    const gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    WHEN("Calling GetNodes") {
      auto nodes = g1.GetNodes();
      THEN("Node vector is return in ascending order") {
        std::sort(v1.begin(), v1.end());
        for (int i = 0; i < 3; ++i) {
          REQUIRE(nodes[i] == v1[i]);
        }
      }
    }
  }
}

SCENARIO("Testing GetConnected") {
  GIVEN("A graph that does not include src") {
    std::vector<std::string> v1{"Maths", "Hello", "Army"};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    std::string src{"Cars"};
    WHEN("Calling GetConnected") {
      THEN("Exception is thrown") {
        REQUIRE_THROWS_AS(g1.GetConnected(src), std::out_of_range);
        REQUIRE_THROWS_WITH(g1.GetConnected(src),
                            "Cannot call Graph::GetConnected if src doesn't exist in the graph");
      }
    }
  }
  GIVEN("A graph that does include src with no outgoing edges") {
    std::vector<std::string> v1{"Maths", "Hello", "Army"};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    std::string src{"Hello"};
    WHEN("Calling GetConnected") {
      auto nodes = g1.GetConnected(src);
      THEN("Empty nodes vector returned") { REQUIRE(nodes.size() == 0); }
    }
  }
  GIVEN("A graph that does include src with outgoing edges") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    WHEN("Calling GetConnected") {
      auto nodes = g1.GetConnected(s1);
      THEN("Nodes vector returned") {
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0] == s2);
      }
    }
  }
  GIVEN("A graph that does include src with outgoing edges") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s1, s2, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    WHEN("Calling GetConnected with more that one edge to the same node") {
      auto nodes = g1.GetConnected(s1);
      THEN("Nodes vector returned, connected node should only occur in the vector once") {
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0] == s2);
      }
    }
  }
  GIVEN("A const graph that does not include src") {
    std::vector<std::string> v1{"Maths", "Hello", "Army"};
    const gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    std::string src{"Cars"};
    WHEN("Calling GetConnected") {
      THEN("Exception is thrown") {
        REQUIRE_THROWS_AS(g1.GetConnected(src), std::out_of_range);
        REQUIRE_THROWS_WITH(g1.GetConnected(src),
                            "Cannot call Graph::GetConnected if src doesn't exist in the graph");
      }
    }
  }
  GIVEN("A const graph that does include src with no outgoing edges") {
    std::vector<std::string> v1{"Maths", "Hello", "Army"};
    const gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    std::string src{"Hello"};
    WHEN("Calling GetConnected") {
      auto nodes = g1.GetConnected(src);
      THEN("Empty nodes vector returned") { REQUIRE(nodes.size() == 0); }
    }
  }
  GIVEN("A const graph that does include src with outgoing edges") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    const gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    WHEN("Calling GetConnected") {
      auto nodes = g1.GetConnected(s1);
      THEN("Nodes vector returned") {
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0] == s2);
      }
    }
  }
}

// ==== Operator << ====
SCENARIO("Testing operator <<") {
  GIVEN("A graph with a node but no edges") {
    gdwg::Graph<std::string, int> g{"Cactus"};
    WHEN("Stream graph to string stream") {
      std::ostringstream oss;
      oss << g;
      THEN("String should be equivalent to Cactus()") { REQUIRE(oss.str() == "Cactus (\n)\n"); }
    }
  }
  GIVEN("A graph with a node but no edges") {
    std::string s1{"Cactus"};
    std::string s2{"sand"};
    auto e1 = std::make_tuple(s1, s2, 3);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Stream graph to string stream") {
      std::ostringstream oss;
      oss << g;
      THEN("String should be equivalent to Cactus()") {
        REQUIRE(oss.str() == "Cactus (\n"
                             "  sand | 3\n"
                             ")\n"
                             "sand (\n"
                             ")\n");
      }
    }
  }
}

// ==== Clear ====
SCENARIO("Testing Clear Method") {
  GIVEN("A graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Calling clear") {
      g.Clear();
      THEN("Graph is empty") {
        auto v = g.GetNodes();
        REQUIRE(v.size() == 0);
      }
    }
  }
}

SCENARIO("Testing Move Constructor") {
  GIVEN("A constructed graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Constructing DG from move DG") {
      gdwg::Graph<std::string, int> gMove{std::move(g)};
      THEN("All strings in vector are valid nodes in graph") {
        for (int i = 0; i < static_cast<int>(v1.size()); ++i) {
          REQUIRE(gMove.IsNode(std::get<0>(v1[i])));
          REQUIRE(gMove.IsNode(std::get<1>(v1[i])));
        }
      }
      AND_THEN("Hello and how are connected by a weight of 4") {
        REQUIRE(gMove.IsConnected(s1, s2));
        auto v2 = gMove.GetWeights(s1, s2);
        REQUIRE(v2[0] == 4);
      }
      AND_THEN("how and cow are connected by a weight of 7") {
        REQUIRE(gMove.IsConnected(s2, s3));
        auto v2 = gMove.GetWeights(s2, s3);
        REQUIRE(v2[0] == 7);
      }
      AND_THEN("g is empty") {
        auto v = g.GetNodes();
        REQUIRE(v.size() == 0);
      }
    }
  }
}

// ==== Iterator Methods ====
// This test case is aim to test ++ operator with cbegin
// it tries to iterate Empty graph, Graph with 1 Node and Graph with 3 Nodes
// This Test doesn't include testing ordering
SCENARIO("Testing edge const_iterator's cbegin and cend with ++ operator") {
  GIVEN("A Graph with <String, int> Type") {
    gdwg::Graph<std::string, int> g;
    AND_GIVEN("No Nodes and a beginning iterator") {
      auto it = g.cbegin();
      auto it_end = g.cend();
      WHEN("we iterate an empty graph") {
        THEN("end should be end()") {
          REQUIRE(it == g.cbegin());
          REQUIRE(it == g.cend());
          REQUIRE(it_end == g.cbegin());
          REQUIRE(it_end == g.cend());
        }
        THEN("Doing ++ operation on iterator should return nothing") {
          REQUIRE(++it == g.cend());
          REQUIRE(++it == g.cbegin());
        }
      }
    }
    AND_GIVEN("a Nodes, no Edges") {
      g.InsertNode("hello");
      auto it = g.cbegin();
      auto it_end = g.cend();
      WHEN("We iterate a graph with 1 nodes with no edges") {
        THEN("The end should be begin ") {
          REQUIRE(it == g.cend());        // Because this is an empty node
          REQUIRE(it_end == g.cbegin());  // Because this is an empty node
        }
        THEN("Doing ++ operator should return end") {
          REQUIRE(++it == g.cend());
          REQUIRE(++it_end == g.cend());
          REQUIRE(++it_end == g.cbegin());
          REQUIRE(++it == g.cbegin());
        }
      }
    }
    AND_GIVEN("3 Nodes, A B C declared in order of A, B, C") {
      auto A = "Are";
      auto B = "Bye";
      auto C = "Cute";
      g.InsertNode(A);
      g.InsertNode(B);
      g.InsertNode(C);
      AND_GIVEN("An iterator and B has Empty outgoing Edge ") {
        AND_GIVEN("A has 2 Edge, C Has 1 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return A-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return A -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == A);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == C);
                REQUIRE(to1 == B);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 2 Edge, C Has 2 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return A-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return A -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == A);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill cend of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 3);
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == C);
                REQUIRE(to1 == B);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 1 Edge, C Has 2 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return A-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return C -> A with edge 3 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == C);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == C);
                REQUIRE(to1 == B);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
      }

      AND_GIVEN("An iterator and A has Empty outgoing Edge. B, C Has Edges") {
        AND_GIVEN("B has 1 Edge, C Has 2 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return B-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return C -> A with edge 3 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == C);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == C);
                REQUIRE(to1 == B);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("B has 2 Edge, C Has 1 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, A, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return B-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return B -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == B);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }

        AND_GIVEN("B has 2 Edge, C Has 2 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return B-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return B -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == B);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 3);
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == C);
                REQUIRE(to1 == B);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
      }

      AND_GIVEN("An iterator and A, C has Empty outgoing Edge. B Has Edges") {
        AND_GIVEN("B with 1 Edge") {
          g.InsertEdge(B, A, 4);
          auto it = g.cbegin();
          THEN("The cbegin() should return B-> A with edge 4") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == A);
            REQUIRE(weight == 4);
          }

          AND_WHEN("Try to ++ at the end of the edge") {
            it++;
            THEN("It should be just g.cend()") { REQUIRE(it == g.cend()); }
          }
        }
        AND_GIVEN("B with 2 Edge") {
          g.InsertEdge(B, A, 4);
          g.InsertEdge(B, C, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return B-> A with edge 4") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == A);
            REQUIRE(weight == 4);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return B -> C with edge 3 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == B);
              REQUIRE(to == C);
              REQUIRE(weight == 3);
            }
            AND_WHEN("Try to ++ at the end of the edge") {
              ++it;
              THEN("It should be just g.cend()") {
                REQUIRE(it == g.cend());  // Shouldn't change anything
              }
            }
          }
        }
      }

      AND_GIVEN("An iterator and C has Empty outgoing Edge, A, B Has Edges") {
        AND_GIVEN("A has 2 Edges, B has 1 Edges") {
          g.InsertEdge(A, B, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(B, A, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return A-> B with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return A -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == A);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == B);
                REQUIRE(to == A);
                REQUIRE(weight == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 1 Edges, B has 2 Edges") {
          g.InsertEdge(A, B, 3);
          g.InsertEdge(B, A, 3);
          g.InsertEdge(B, C, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return A-> B with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return B -> A with edge 3 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == B);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == B);
                REQUIRE(to1 == C);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 2 Edges, B has 2 Edges") {
          g.InsertEdge(A, B, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(B, A, 3);
          g.InsertEdge(B, C, 3);
          auto it = g.cbegin();
          THEN("The cbegin() should return A-> B with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return A -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == A);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and cend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == B);
                REQUIRE(to == A);
                REQUIRE(weight == 3);
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == B);
                REQUIRE(to1 == C);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.cend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.cend()") {
                    REQUIRE(it == g.cend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Same Scenario as above but I Will iterator thorugh the end and -- back
SCENARIO("Testing edge const_iterator's cbegin and cend with -- operator") {
  GIVEN("A Graph with <String, int> Type") {
    gdwg::Graph<std::string, int> g;
    AND_GIVEN("No Nodes and a beginning iterator") {
      auto it = g.cend();
      WHEN("we iterate an empty graph") {
        THEN("end should be end()") {
          REQUIRE(it == g.cend());
          REQUIRE(it == g.cbegin());
        }
        THEN("Doing -- operation on iterator should return nothing") {
          --it;
          REQUIRE(it == g.cend());
          --it;
          REQUIRE(it == g.cbegin());
        }
      }
    }
    AND_GIVEN("a Nodes, no Edges with a cbegin iterator") {
      g.InsertNode("hello");
      auto it = g.cbegin();
      WHEN("We iterate a graph with 1 nodes with no edges") {
        THEN("The end should be begin ") {
          REQUIRE(it == g.cend());  // Because this is an empty node
        }
        THEN("Doing -- operator should return end") {
          REQUIRE(--it == g.cend());
          REQUIRE(--it == g.cbegin());
        }
      }
    }
    AND_GIVEN("3 Nodes, A B C declared in order of A, B, C") {
      auto A = "Are";
      auto B = "Bye";
      auto C = "Cute";
      g.InsertNode(A);
      g.InsertNode(B);
      g.InsertNode(C);
      AND_GIVEN("An iterator and B has Empty outgoing Edge ") {
        AND_GIVEN("A has 2 Edge, C Has 1 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return C-> B with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return A -> C with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == A);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == A);
                  REQUIRE(to1 == C);
                  REQUIRE(weight1 == 3);
                  --it;
                  REQUIRE(it == g.cbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.cbegin()") {
                      REQUIRE(it == g.cbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 1 Edge, C Has 2 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(C, A, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return C-> B with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return C -> A with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == A);
                  REQUIRE(to1 == C);
                  REQUIRE(weight1 == 3);
                  --it;
                  REQUIRE(it == g.cbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.cbegin()") {
                      REQUIRE(it == g.cbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 2 Edge, C Has 2 Edge") {
          g.InsertEdge(A, B, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          ++it;
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return C-> B with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return C -> A with edge 3 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 3);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == A);
                  REQUIRE(to1 == C);
                  REQUIRE(weight1 == 4);
                  --it;
                  auto& from2 = std::get<0>(*it);
                  auto& to2 = std::get<1>(*it);
                  auto& weight2 = std::get<2>(*it);
                  REQUIRE(from2 == A);
                  REQUIRE(to2 == B);
                  REQUIRE(weight2 == 3);
                  --it;
                  REQUIRE(it == g.cbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.cbegin()") {
                      REQUIRE(it == g.cbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
      }

      AND_GIVEN("An iterator and A has Empty outgoing Edge. B, C Has Edges") {
        AND_GIVEN("B has 2 Edge, C Has 1 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return C-> B with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return B -> C with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == B);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == B);
                  REQUIRE(to1 == C);
                  REQUIRE(weight1 == 3);
                  --it;
                  REQUIRE(it == g.cbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.cbegin()") {
                      REQUIRE(it == g.cbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("B has 1 Edge, C Has 2 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(C, A, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return C-> B with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return C -> A with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == B);
                  REQUIRE(to1 == C);
                  REQUIRE(weight1 == 3);
                  --it;
                  REQUIRE(it == g.cbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.cbegin()") {
                      REQUIRE(it == g.cbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("B has 2 Edge, C Has 2 Edge") {
          g.InsertEdge(B, B, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.cbegin();
          ++it;
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return C-> B with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return C -> A with edge 3 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 3);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == B);
                  REQUIRE(to1 == C);
                  REQUIRE(weight1 == 4);
                  --it;
                  auto& from2 = std::get<0>(*it);
                  auto& to2 = std::get<1>(*it);
                  auto& weight2 = std::get<2>(*it);
                  REQUIRE(from2 == B);
                  REQUIRE(to2 == B);
                  REQUIRE(weight2 == 3);
                  --it;
                  REQUIRE(it == g.cbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.cbegin()") {
                      REQUIRE(it == g.cbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

SCENARIO("Testing edge const_iterator's crbegin and crend with ++ operator") {
  GIVEN("A Graph with <String, int> Type") {
    gdwg::Graph<std::string, int> g;

    AND_GIVEN("No Nodes and a beginning iterator") {
      auto it = g.crbegin();
      WHEN("we iterate an empty graph") {
        THEN("crend should be crend()") {
          REQUIRE(it == g.crend());
          REQUIRE(it == g.crbegin());
        }
        THEN("Doing ++ operation on iterator should return nothing") {
          REQUIRE(++it == g.crend());
          REQUIRE(++it == g.crbegin());
        }
      }
    }
    AND_GIVEN("a Nodes, no Edges") {
      g.InsertNode("hello");
      auto it = g.crbegin();
      WHEN("We iterate a graph with 1 nodes with no edges") {
        THEN("The end should be begin ") {
          REQUIRE(it == g.crend());  // Because this is an empty node
        }
        THEN("Doing ++ operator should return end") {
          REQUIRE(++it == g.crend());
          REQUIRE(++it == g.crbegin());
        }
      }
    }

    AND_GIVEN("3 Nodes, A B C declared in order of A, B, C") {
      auto A = "Are";
      auto B = "Bye";
      auto C = "Cute";
      g.InsertNode(A);
      g.InsertNode(B);
      g.InsertNode(C);
      AND_GIVEN("An iterator and B has Empty outgoing Edge ") {
        AND_GIVEN("A has 2 Edge, C Has 1 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          THEN("The begin() should return C-> B with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return A -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == A);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == A);
                REQUIRE(to1 == C);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 2 Edge, C Has 2 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return C -> B with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return C -> A with edge 3 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == C);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == A);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == A);
                REQUIRE(to1 == C);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 1 Edge, C Has 2 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return C-> B with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return C -> A with edge 3 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == C);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == A);
                REQUIRE(to1 == C);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
      }

      AND_GIVEN("An iterator and A has Empty outgoing Edge. B, C Has Edges") {
        AND_GIVEN("B has 1 Edge, C Has 2 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return C-> B with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return C -> A with edge 3 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == C);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == B);
                REQUIRE(to1 == C);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("B has 2 Edge, C Has 1 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, A, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return C-> A with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == A);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return B -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == B);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == B);
                REQUIRE(to == C);
                REQUIRE(weight == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("B has 2 Edge, C Has 2 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return B-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == C);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return C -> A with edge 3 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == C);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == B);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == B);
                REQUIRE(to1 == C);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
      }

      AND_GIVEN("An iterator and A, C has Empty outgoing Edge. B Has Edges") {
        AND_GIVEN("B with 1 Edge") {
          g.InsertEdge(B, A, 4);
          auto it = g.crbegin();
          THEN("The crbegin() should return B-> A with edge 4") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == A);
            REQUIRE(weight == 4);
          }

          AND_WHEN("Try to ++ at the end of the edge") {
            it++;
            THEN("It should be just g.crend()") { REQUIRE(it == g.crend()); }
          }
        }
        AND_GIVEN("B with 2 Edge") {
          g.InsertEdge(B, A, 4);
          g.InsertEdge(B, C, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return B-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return B -> A with edge 4") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == B);
              REQUIRE(to == A);
              REQUIRE(weight == 4);
            }
            AND_WHEN("Try to ++ at the end of the edge") {
              ++it;
              THEN("It should be just g.crend()") {
                REQUIRE(it == g.crend());  // Shouldn't change anything
              }
            }
          }
        }
      }

      AND_GIVEN("An iterator and C has Empty outgoing Edge, A, B Has Edges") {
        AND_GIVEN("A has 2 Edges, B has 1 Edges") {
          g.InsertEdge(A, B, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(B, A, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return B -> A with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == A);
            REQUIRE(weight == 3);
          }

          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return A -> C with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == A);
              REQUIRE(to == C);
              REQUIRE(weight == 4);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == A);
                REQUIRE(to == B);
                REQUIRE(weight == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 1 Edges, B has 2 Edges") {
          g.InsertEdge(A, B, 3);
          g.InsertEdge(B, A, 3);
          g.InsertEdge(B, C, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return B-> C with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return B -> A with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == B);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == A);
                REQUIRE(to1 == B);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 2 Edges, B has 2 Edges") {
          g.InsertEdge(A, B, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(B, A, 3);
          g.InsertEdge(B, C, 3);
          auto it = g.crbegin();
          THEN("The crbegin() should return A-> B with edge 3") {
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
          }
          WHEN("DOING ++ operator once") {
            ++it;
            THEN("It should return B -> A with edge 4 ") {
              auto& from = std::get<0>(*it);
              auto& to = std::get<1>(*it);
              auto& weight = std::get<2>(*it);
              REQUIRE(from == B);
              REQUIRE(to == A);
              REQUIRE(weight == 3);
            }
            AND_WHEN("continue DOING ++ operator untill end of edge ") {
              THEN("It should return the edge correctly and crend() at the end ") {
                ++it;
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == A);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
                ++it;
                auto& from1 = std::get<0>(*it);
                auto& to1 = std::get<1>(*it);
                auto& weight1 = std::get<2>(*it);
                REQUIRE(from1 == A);
                REQUIRE(to1 == B);
                REQUIRE(weight1 == 3);
                ++it;
                REQUIRE(it == g.crend());
                AND_WHEN("Try to ++ at the end of the edge") {
                  ++it;
                  THEN("It should be just g.crend()") {
                    REQUIRE(it == g.crend());  // Shouldn't change anything
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

SCENARIO("Testing edge const_iterator's crbegin and crend with -- operator") {
  GIVEN("A Graph with <String, int> Type") {
    gdwg::Graph<std::string, int> g;
    AND_GIVEN("No Nodes and a beginning iterator") {
      auto it = g.crend();
      WHEN("we iterate an empty graph") {
        THEN("end should be end()") {
          REQUIRE(it == g.crend());
          REQUIRE(it == g.crbegin());
        }
        THEN("Doing -- operation on iterator should return nothing") {
          --it;
          REQUIRE(it == g.crend());
          --it;
          REQUIRE(it == g.crbegin());
        }
      }
    }
    AND_GIVEN("a Nodes, no Edges with a cbegin iterator") {
      g.InsertNode("hello");
      auto it = g.crbegin();
      WHEN("We iterate a graph with 1 nodes with no edges") {
        THEN("The end should be begin ") {
          REQUIRE(it == g.crend());  // Because this is an empty node
        }
        THEN("Doing -- operator should return end") {
          REQUIRE(--it == g.crend());
          REQUIRE(--it == g.crbegin());
        }
      }
    }
    AND_GIVEN("3 Nodes, A B C declared in order of A, B, C") {
      auto A = "Are";
      auto B = "Bye";
      auto C = "Cute";
      g.InsertNode(A);
      g.InsertNode(B);
      g.InsertNode(C);
      AND_GIVEN("An iterator and B has Empty outgoing Edge ") {
        AND_GIVEN("A has 2 Edge, C Has 1 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return A-> C with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return A -> C with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == A);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == C);
                  REQUIRE(to1 == B);
                  REQUIRE(weight1 == 3);
                  --it;
                  REQUIRE(it == g.crbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.crbegin()") {
                      REQUIRE(it == g.crbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 1 Edge, C Has 2 Edge") {
          g.InsertEdge(A, C, 3);
          g.InsertEdge(C, A, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return A-> C with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return A -> C with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == C);
                  REQUIRE(to1 == B);
                  REQUIRE(weight1 == 3);
                  --it;
                  REQUIRE(it == g.crbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.crbegin()") {
                      REQUIRE(it == g.crbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("A has 2 Edge, C Has 2 Edge") {
          g.InsertEdge(A, B, 3);
          g.InsertEdge(A, C, 4);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          ++it;
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return A-> B with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == A);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return A -> C with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == A);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == C);
                  REQUIRE(to1 == A);
                  REQUIRE(weight1 == 3);
                  --it;
                  auto& from2 = std::get<0>(*it);
                  auto& to2 = std::get<1>(*it);
                  auto& weight2 = std::get<2>(*it);
                  REQUIRE(from2 == C);
                  REQUIRE(to2 == B);
                  REQUIRE(weight2 == 3);
                  --it;
                  REQUIRE(it == g.crbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.crbegin()") {
                      REQUIRE(it == g.crbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
      }

      AND_GIVEN("An iterator and A has Empty outgoing Edge. B, C Has Edges") {
        AND_GIVEN("B has 2 Edge, C Has 1 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return B-> C with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return B -> C with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == B);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == C);
                  REQUIRE(to1 == B);
                  REQUIRE(weight1 == 3);
                  --it;
                  REQUIRE(it == g.crbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.crbegin()") {
                      REQUIRE(it == g.crbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("B has 1 Edge, C Has 2 Edge") {
          g.InsertEdge(B, C, 3);
          g.InsertEdge(C, A, 4);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return B-> C with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == C);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return C -> A with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == C);
                REQUIRE(to == A);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == C);
                  REQUIRE(to1 == B);
                  REQUIRE(weight1 == 3);
                  --it;
                  REQUIRE(it == g.crbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.crbegin()") {
                      REQUIRE(it == g.crbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
        AND_GIVEN("B has 2 Edge, C Has 2 Edge") {
          g.InsertEdge(B, B, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, A, 3);
          g.InsertEdge(C, B, 3);
          auto it = g.crbegin();
          ++it;
          ++it;
          ++it;
          ++it;  // go to the end
          THEN("The --it should return B-> B with edge 3") {
            --it;
            auto& from = std::get<0>(*it);
            auto& to = std::get<1>(*it);
            auto& weight = std::get<2>(*it);
            REQUIRE(from == B);
            REQUIRE(to == B);
            REQUIRE(weight == 3);
            WHEN("continue --it") {
              --it;
              THEN("It should return B -> C with edge 4 ") {
                auto& from = std::get<0>(*it);
                auto& to = std::get<1>(*it);
                auto& weight = std::get<2>(*it);
                REQUIRE(from == B);
                REQUIRE(to == C);
                REQUIRE(weight == 4);
              }
              AND_WHEN("continue DOING -- operator untill end of edge ") {
                THEN("It should return the edge correctly and cend() at the end ") {
                  --it;
                  auto& from1 = std::get<0>(*it);
                  auto& to1 = std::get<1>(*it);
                  auto& weight1 = std::get<2>(*it);
                  REQUIRE(from1 == C);
                  REQUIRE(to1 == A);
                  REQUIRE(weight1 == 3);
                  --it;
                  auto& from2 = std::get<0>(*it);
                  auto& to2 = std::get<1>(*it);
                  auto& weight2 = std::get<2>(*it);
                  REQUIRE(from2 == C);
                  REQUIRE(to2 == B);
                  REQUIRE(weight2 == 3);
                  --it;
                  REQUIRE(it == g.crbegin());
                  AND_WHEN("Try to -- at the end of the edge") {
                    --it;
                    THEN("It should be just g.crbegin()") {
                      REQUIRE(it == g.crbegin());  // Shouldn't change anything
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

SCENARIO("Testing for Const_Iterator Ordering") {
  GIVEN("Int types only for SrcNode, DstNode and Edge") {
    gdwg::Graph<int, int> g;
    auto A = 3;
    auto B = 5;
    auto C = 7;
    g.InsertNode(A);
    g.InsertNode(B);
    g.InsertNode(C);
    WHEN("We Gives some Edges in reverse order, making dstNode and Edge the same") {
      g.InsertEdge(C, C, 0);
      g.InsertEdge(B, C, 0);
      g.InsertEdge(A, C, 0);
      THEN("The Ordering should be A->B->C") {
        auto it = g.cbegin();
        REQUIRE(std::get<0>(*it) == A);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 0);
        ++it;
        REQUIRE(std::get<0>(*it) == B);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 0);
        ++it;
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 0);
      }
    }
    WHEN("We Gives some Edges in reverse order, making srcNode and Edge the same") {
      g.InsertEdge(C, C, 0);
      g.InsertEdge(C, B, 0);
      g.InsertEdge(C, A, 0);
      THEN("The Ordering should be A->B->C") {
        auto it = g.cbegin();
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == A);
        REQUIRE(std::get<2>(*it) == 0);
        ++it;
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == B);
        REQUIRE(std::get<2>(*it) == 0);
        ++it;
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 0);
      }
    }
    WHEN("We Gives some Edges in reverse order, making srcNode and DstNode the same") {
      g.InsertEdge(C, C, 5);
      g.InsertEdge(C, C, 4);
      g.InsertEdge(C, C, 3);
      THEN("The Ordering should be A->B->C") {
        auto it = g.cbegin();
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 3);
        ++it;
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 4);
        ++it;
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 5);
      }
    }
    WHEN("We Gives some Edges in reverse order") {
      g.InsertEdge(C, C, 1);
      g.InsertEdge(C, B, 10);
      g.InsertEdge(C, B, 5);  // 8
      g.InsertEdge(B, C, 6);
      g.InsertEdge(B, B, 2);
      g.InsertEdge(B, A, 9);
      g.InsertEdge(A, C, 5);  // 4
      g.InsertEdge(A, C, 4);
      g.InsertEdge(A, B, 3);
      g.InsertEdge(A, A, 2);
      THEN("The Ordering should be A->B->C") {
        auto it = g.cbegin();
        REQUIRE(std::get<0>(*it) == A);
        REQUIRE(std::get<1>(*it) == A);
        REQUIRE(std::get<2>(*it) == 2);
        ++it;
        REQUIRE(std::get<0>(*it) == A);
        REQUIRE(std::get<1>(*it) == B);
        REQUIRE(std::get<2>(*it) == 3);
        ++it;
        REQUIRE(std::get<0>(*it) == A);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 4);
        ++it;
        REQUIRE(std::get<0>(*it) == A);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 5);
        ++it;
        REQUIRE(std::get<0>(*it) == B);
        REQUIRE(std::get<1>(*it) == A);
        REQUIRE(std::get<2>(*it) == 9);
        ++it;
        REQUIRE(std::get<0>(*it) == B);
        REQUIRE(std::get<1>(*it) == B);
        REQUIRE(std::get<2>(*it) == 2);

        ++it;
        REQUIRE(std::get<0>(*it) == B);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 6);
        ++it;
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == B);
        REQUIRE(std::get<2>(*it) == 5);
        ++it;
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == B);
        REQUIRE(std::get<2>(*it) == 10);
        ++it;
        REQUIRE(std::get<0>(*it) == C);
        REQUIRE(std::get<1>(*it) == C);
        REQUIRE(std::get<2>(*it) == 1);
      }
    }
  }
}

// ==== Find ====
SCENARIO("Testing find") {
  GIVEN("A graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Calling find with src not in the graph") {
      THEN("Returns cend()") { REQUIRE(g.find("cat", s2, 4) == g.cend()); }
    }
    WHEN("Calling find with dst not in the graph") {
      THEN("Returns cend()") { REQUIRE(g.find(s1, "dog", 4) == g.cend()); }
    }
    WHEN("Calling find with weight not in the graph") {
      THEN("Returns cend()") { REQUIRE(g.find(s1, s2, 19) == g.cend()); }
    }
    WHEN("Calling find with valid parameters, edge exists in the graph") {
      auto it = g.find(s1, s2, 4);
      THEN("Returns value != cend()") { REQUIRE(it != g.cend()); }
      AND_THEN("Values of the tuple corresponds to the edge looked up") {
        REQUIRE(std::get<0>(*it) == s1);
        REQUIRE(std::get<1>(*it) == s2);
        REQUIRE(std::get<2>(*it) == 4);
      }
    }
  }
  GIVEN("A const graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    const gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Calling find with src not in the graph") {
      THEN("Returns cend()") { REQUIRE(g.find("cat", s2, 4) == g.cend()); }
    }
    WHEN("Calling find with dst not in the graph") {
      THEN("Returns cend()") { REQUIRE(g.find(s1, "dog", 4) == g.cend()); }
    }
    WHEN("Calling find with weight not in the graph") {
      THEN("Returns cend()") { REQUIRE(g.find(s1, s2, 19) == g.cend()); }
    }
    WHEN("Calling find with valid parameters, edge exists in the graph") {
      auto it = g.find(s1, s2, 4);
      THEN("Returns value != cend()") { REQUIRE(it != g.cend()); }
      AND_THEN("Values of the tuple corresponds to the edge looked up") {
        REQUIRE(std::get<0>(*it) == s1);
        REQUIRE(std::get<1>(*it) == s2);
        REQUIRE(std::get<2>(*it) == 4);
      }
    }
  }
}

// ==== Erase methods ====
SCENARIO("Testing Erase of edges") {
  GIVEN("A graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Calling erase with src not in the graph") {
      THEN("Returns false") { REQUIRE(!g.erase("cat", s2, 4)); }
    }
    WHEN("Calling erase with dst not in the graph") {
      THEN("Returns false") { REQUIRE(!g.erase(s1, "bark", 4)); }
    }
    WHEN("Calling erase with weight not in the graph") {
      THEN("Returns false") { REQUIRE(!g.erase(s1, s2, 19)); }
    }
    WHEN("Calling erase with valid parameters, edge exists in the graph") {
      THEN("Returns true, edge is removed") {
        auto result = g.erase(s1, s2, 4);
        REQUIRE(result);
        REQUIRE(!g.IsConnected(s1, s2));
      }
    }
  }
}

SCENARIO("Testing Erase of iterators of edges") {
  GIVEN("A graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Calling erase it points to valid edge") {
      auto it = g.erase(g.cbegin());
      THEN("Point to the next edge") {
        REQUIRE(std::get<0>(*it) == s2);
        REQUIRE(std::get<1>(*it) == s3);
        REQUIRE(std::get<2>(*it) == 7);
        REQUIRE(!g.IsConnected(s1, s2));
      }
    }
  }
  GIVEN("A graph with only one edge") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Calling erase it points to valid edge") {
      auto it = g.erase(g.cbegin());
      THEN("Returns cend(), edge erased") {
        REQUIRE(it == g.cend());
        REQUIRE(!g.IsConnected(s1, s2));
      }
    }
  }
  GIVEN("2 Graphs") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    auto v2 = std::vector<std::tuple<std::string, std::string, int>>{e2};
    gdwg::Graph<std::string, int> g2{v2.begin(), v2.end()};
    WHEN("Calling erase to an invalid edge") {
      auto it = g.erase(g2.cbegin());
      THEN("Returns cend()") { REQUIRE(it == g.cend()); }
    }
  }
}

// ==== DeleteNode ===
SCENARIO("Testing DeleteNode") {
  GIVEN("A graph with just nodes") {
    gdwg::Graph<std::string, int> g{"Hello", "little", "cat"};
    WHEN("Deleting node that exists in the graph") {
      THEN("Returns true, and node is deleted") {
        auto result = g.DeleteNode("Hello");
        REQUIRE(result);
        REQUIRE(!g.IsNode("Hello"));
      }
    }
  }
  GIVEN("A graph with just nodes") {
    gdwg::Graph<std::string, int> g{"Hello", "little", "cat"};
    WHEN("Deleting node that does not exists in the graph") {
      THEN("Returns false") {
        auto result = g.DeleteNode("duck");
        REQUIRE(!result);
      }
    }
    GIVEN("A graph with nodes and edges") {
      std::string s1{"Hello"};
      std::string s2{"how"};
      std::string s3{"cow"};
      auto e1 = std::make_tuple(s1, s2, 4);
      auto e2 = std::make_tuple(s2, s3, 7);
      auto e3 = std::make_tuple(s1, s2, 5);
      auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
      gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
      WHEN("Deleting valid node with edges") {
        auto result = g.DeleteNode("how");
        THEN("Returns true, and node is deleted and all edges connected to it") {
          REQUIRE(result);
          REQUIRE(!g.IsNode("how"));
          auto v = g.GetConnected("Hello");
          REQUIRE(v.size() == 0);
        }
      }
    }
  }
}

// ==== Replace ====
SCENARIO("Testing Replace Method") {
  GIVEN("A populated graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Replace a node that does not exist in the graph") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(g.Replace("cat", "dog"), std::runtime_error);
        REQUIRE_THROWS_WITH(g.Replace("cat", "dog"),
                            "Cannot call Graph::Replace on a node that doesn't exist");
      }
    }
  }
  GIVEN("A populated graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Replacing a valid node with one that exists in the graph already") {
      THEN("Replace does not occur, returns false") { REQUIRE(!g.Replace(s1, s2)); }
    }
  }
  GIVEN("A populated graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s2, s1, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Replacing a valid node with one that does not exist in the graph already") {
      auto result = g.Replace(s1, "Bye");
      THEN("Replace does occur, returns true") {
        REQUIRE(result);
        REQUIRE(g.IsNode("Bye"));
        REQUIRE(!g.IsNode(s1));
      }
      AND_THEN("Outgoing edges are changed") { REQUIRE(g.IsConnected("Bye", s2)); }
      AND_THEN("incoming edges are changed") { REQUIRE(g.IsConnected(s2, "Bye")); }
    }
  }
}

SCENARIO("Testing MergeReplace Method") {
  GIVEN("A populated graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Replace a node that does not exist in the graph") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(g.MergeReplace("cat", s2), std::runtime_error);
        REQUIRE_THROWS_WITH(
            g.MergeReplace("cat", s2),
            "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }
  }
  GIVEN("A populated graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s2, s1, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Replacing a valid node with one that does not exists in the graph already") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(g.MergeReplace(s1, "moo"), std::runtime_error);
        REQUIRE_THROWS_WITH(
            g.MergeReplace(s1, "moo"),
            "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }
  }
  GIVEN("A populated graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g{v1.begin(), v1.end()};
    WHEN("Replacing a valid node with one that exists in the graph already") {
      g.MergeReplace(s1, s2);
      THEN("Replace occurs") { REQUIRE(!g.IsNode(s1)); }
      AND_THEN("how has 2 edges to itself of 4 and 5") {
        REQUIRE(g.IsConnected(s2, s2));
        auto v = g.GetWeights(s2, s2);
        REQUIRE(v[0] == 4);
        REQUIRE(v[1] == 5);
      }
    }
  }
}

// ==== Friends =====
SCENARIO("Testing equals operator") {
  GIVEN("Two identical graphs of nodes") {
    gdwg::Graph<std::string, int> g1{"Hello", "little", "cat"};
    gdwg::Graph<std::string, int> g2{g1};
    WHEN("Comparing if graphs are equal") {
      THEN("Returns true") { REQUIRE(g1 == g2); }
    }
  }
  GIVEN("Two non-identical graphs of nodes") {
    gdwg::Graph<std::string, int> g1{"Hello", "little", "cat"};
    gdwg::Graph<std::string, int> g2{"Hello", "little", "cow"};
    WHEN("Comparing if graphs are equal") {
      THEN("Returns false") { REQUIRE(!(g1 == g2)); }
    }
  }
  GIVEN("Two identical graphs of nodes and edges") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    gdwg::Graph<std::string, int> g2{g1};
    WHEN("Comparing if graphs are equal") {
      THEN("Returns true") { REQUIRE(g1 == g2); }
    }
  }
  GIVEN("Two non-identical graphs of nodes and edges") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    auto v2 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g2{v2.begin(), v2.end()};
    WHEN("Comparing if graphs are equal") {
      THEN("Returns false") { REQUIRE(!(g1 == g2)); }
    }
  }
}

SCENARIO("Testing not equals operator") {
  GIVEN("Two identical graphs of nodes") {
    gdwg::Graph<std::string, int> g1{"Hello", "little", "cat"};
    gdwg::Graph<std::string, int> g2{g1};
    WHEN("Comparing if graphs are not equal") {
      THEN("Returns false") { REQUIRE(!(g1 != g2)); }
    }
  }
  GIVEN("Two non-identical graphs of nodes") {
    gdwg::Graph<std::string, int> g1{"Hello", "little", "cat"};
    gdwg::Graph<std::string, int> g2{"Hello", "little", "cow"};
    WHEN("Comparing if graphs are equal") {
      THEN("Returns true") { REQUIRE(g1 != g2); }
    }
  }
  GIVEN("Two identical graphs of nodes and edges") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    gdwg::Graph<std::string, int> g2{g1};
    WHEN("Comparing if graphs are equal") {
      THEN("Returns false") { REQUIRE(!(g1 != g2)); }
    }
  }
  GIVEN("Two non-identical graphs of nodes and edges") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    auto v2 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2};
    gdwg::Graph<std::string, int> g2{v2.begin(), v2.end()};
    WHEN("Comparing if graphs are equal") {
      THEN("Returns true") { REQUIRE(g1 != g2); }
    }
  }
}
// ==== Operators ====
SCENARIO("Testing copy assignment") {
  GIVEN("A graph") {
    std::string s1{"Hello"};
    std::string s2{"how"};
    std::string s3{"cow"};
    auto e1 = std::make_tuple(s1, s2, 4);
    auto e2 = std::make_tuple(s2, s3, 7);
    auto e3 = std::make_tuple(s1, s2, 5);
    auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
    gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
    gdwg::Graph<std::string, int> g2{"Hello"};
    WHEN("Assigning that graph to another graph") {
      g2 = g1;
      THEN("The assigned graph and the initial graph are the same") { REQUIRE(g2 == g1); }
    }
  }
}

SCENARIO("Testing move assignment") {
  std::string s1{"Hello"};
  std::string s2{"how"};
  std::string s3{"cow"};
  auto e1 = std::make_tuple(s1, s2, 4);
  auto e2 = std::make_tuple(s2, s3, 7);
  auto e3 = std::make_tuple(s1, s2, 5);
  auto v1 = std::vector<std::tuple<std::string, std::string, int>>{e1, e2, e3};
  gdwg::Graph<std::string, int> g1{v1.begin(), v1.end()};
  gdwg::Graph<std::string, int> g1Copy{g1};
  gdwg::Graph<std::string, int> g2{"Hello"};
  WHEN("Assigning the move of that graph to another graph") {
    g2 = std::move(g1);
    THEN("The assigned graph and the initial graph are the same") { REQUIRE(g2 == g1Copy); }
    AND_THEN("The initial graph is empty") {
      auto v = g1.GetNodes();
      REQUIRE(v.size() == 0);
    }
  }
}
