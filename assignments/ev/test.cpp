//
// Created by wilson on 9/07/19.
//

#include <iostream>
#include <memory>
#include <algorithm>

int main() {
  double aa = 3;
  double bb = 2;
  double cc = aa = bb;
  std::cout << cc << '\n';
}