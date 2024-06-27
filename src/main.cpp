#include <iostream>

#include "StableMatch.hpp"

void runTest();
void testDefault();

int main() {
  testDefault();
  runTest();
  return 0;
}

// void galeShapley(matchMap suppliers, matchMap consumers) {
//   std::queue<std::pair<strv, prefMap::const_iterator>> queue;
//   std::map<strv, strv> matches;

//   for (auto const &supplier: suppliers) {
//     queue.push({supplier.first, supplier.second.cbegin()});
//   }

//   for(auto here = queue.front(); !queue.empty(); here = queue.front()) {
//     queue.pop();
//     if (matches.find(here.second->second) != matches.cend()) {
//       matches.insert({here.first, here.second->second});
//     } else if (matche) {

//     }
//   }
// }

void testDefault() {
  alg::StableMatch sm(
    "A: X,Y,Z;"
    "B: Y,X,Z;"
    "C: X,Y,Z;"
    ";"
    "X: B,A,C;"
    "Y: A,B,C;"
    "Z: A,B,C;"
  );
  std::cout << sm.toString();
  // sm.getMatches();
}

void runTest() {
  alg::StableMatch sm(
    "A: 1-X,2-Y,4-Z,3-W;"
    "B: 4-X,1-Y,2-Z,3-W;"
    "C: 3-X,1-Y,4-Z,2-W;"
    "D: 2-X,3-Y,1-Z,4-W;"
    ";"
    "X: 1-A,3-B,4-C,2-D;"
    "Y: 3-A,1-B,4-C,2-D;"
    "Z: 3-A,1-B,4-C,2-D;"
    "W: 3-A,2-B,1-C,4-D;",
    true
  );
  std::cout << sm.toString();
}
