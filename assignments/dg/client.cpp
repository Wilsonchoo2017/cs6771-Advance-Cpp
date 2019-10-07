#include <iostream>
#include <string>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  gdwg::Graph<std::string, int> g;
  g.InsertNode("hello");
  g.InsertNode("how");
  g.InsertNode("are");
  g.InsertNode("you?");

  g.InsertEdge("hello", "how", 5);
  g.InsertEdge("hello", "are", 8);
  g.InsertEdge("hello", "are", 2);

  g.InsertEdge("how", "you?", 1);
  g.InsertEdge("how", "hello", 4);
  g.InsertEdge("how", "are", 3);

  g.InsertEdge("are", "you?", 3);
  g.InsertEdge("are", "hello", 5);
  g.InsertEdge("are", "how", 2);

  gdwg::Graph<std::string, int> g;
  auto A = "Abby";
  auto B = "Belle";
  auto C = "Cat";
  g.InsertNode(A);
  g.InsertNode(B);
  g.InsertNode(C);
          g.InsertEdge(B, C, 3);
          g.InsertEdge(B, C, 4);
          g.InsertEdge(C, B, 3);
  auto it = g.crbegin();
  for(; it != g.crend();++it) {
  std::cout<<std::get<0>(*it) << ' ';
  std::cout<<std::get<1>(*it) << ' ';
  std::cout<<std::get<2>(*it) << '\n';
  }
  std::cout << "?\n";
  std::cout<<std::get<0>(*it) << ' ';
  std::cout<<std::get<1>(*it) << ' ';
  std::cout<<std::get<2>(*it) << '\n';
  std::cout << "\n";
  it--;

  
  std::cout<<std::get<0>(*it) << ' ';
  std::cout<<std::get<1>(*it) << ' ';
  std::cout<<std::get<2>(*it) << '\n';
  /* 
  auto it = g.cbegin();

  std::cout << g2 << "\n";

  // This is a structured binding.
  // https://en.cppreference.com/w/cpp/language/structured_binding
  // It allows you to unpack your tuple.
  for (const auto& [from, to, weight] : g) {
    std::cout << from << " -> " << to << " (weight " << weight << ")\n";


  }
}
