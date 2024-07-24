#include "UnitTests.hpp"
#include "Graph.hpp"
#include "Algorithms.hpp"
#include <iostream>
#include <map>
#include <array>

int main() {
  alg::Graph<int, int, int> G;
  for (auto node : {10, 3, 9, 8, 7, 2, 11, 5}) {
    G.addNode(node, 0);
  }
  std::array nfrom = {5, 11, 7, 8, 3, 10};
  auto nit = nfrom.cbegin();
  for (auto node : {11}) {
    G.addEdge(*nit, node, 0);
  }
  ++nit;
  for (auto node : {2, 9, 10}) {
    G.addEdge(*nit, node, 0);
  }
  ++nit;
  for (auto node : {11, 8}) {
    G.addEdge(*nit, node, 0);
  }
  ++nit;
  for (auto node : {9}) {
    G.addEdge(*nit, node, 0);
  }
  ++nit;
  for (auto node : {8, 10}) {
    G.addEdge(*nit, node, 0);
  }
  // ++nit;
  // for (auto node : {11}) {
  //   G.addEdge(*nit, node, 0);
  // }

  for (auto &&u : G) {
    std::cout << u.first << ": ";
    for (auto &&v : u.second) {
      std::cout << v.first->key() << ", ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  auto ret = alg::topoSort(G);
  if (ret.has_value()) {
    for (auto const &v: ret.value()) {
      std::cout << v << ", ";
    }
    std::cout << "\nsorted" << std::endl;
  } else {
    std::cout << "has cycle" << std::endl;
  }

  // tst::stableMatchCases();
  // tst::stableMatchStressTest(2000);
  return 0;
}
