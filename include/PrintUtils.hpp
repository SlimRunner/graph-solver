#pragma once

#include "Graph.hpp"
#include <array>
#include <cstddef> // size_t
#include <deque>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace utl {

namespace gEnum1 {
template <class T, class U = int>
constexpr T operator|(const T lhs, const T rhs) {
  return static_cast<T>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template <class T, class U = int>
constexpr std::ostream &operator<<(std::ostream &ostrm, const T other) {
  ostrm << static_cast<U>(other);
  return ostrm;
}

enum class CellState {
  NONE = 0,
  WEST = 1,
  SOUTH = 1 << 1,
  NORTH = 1 << 2,
  EAST = 1 << 3,
  VERTEX = 1 << 4,
};
} // namespace gEnum1

// THIS FUNCTION IS A STALE WORK IN PROGRESS
template <class K, class T, class W, class It>
std::optional<std::string> printDAG(alg::Graph<K, T, W> graph, It begin,
                                    It end) {
  using namespace gEnum1;
  std::vector<K> keys(begin, end);
  std::map<K, size_t> order;
  std::vector<std::string> keyText;

  // for now this function can only print DAGs

  for (auto const &k : keys) {
    if (!graph.hasNode(k)) {
      return {};
    }
    order.emplace(k, order.size());
    std::ostringstream oss;
    oss << k;
    keyText.push_back(oss.str());
  }

  std::deque<std::vector<CellState>> diagram;
  size_t rootIndex = 0;
  // cells consist of "expandable units"
  // <NODE>[<gap><NODE>...]
  // each <NODE> = <<entry><label/exit>>
  diagram.push_back(std::vector<CellState>(2 + 3 * keys.size(), CellState::NONE));

  for (size_t i = 0, j = 0; j < keys.size(); ++j) {
    for (size_t lf = i, rg = j, idx; rg < keys.size(); ++lf, ++rg) {
      rootIndex = 0;
      if (graph.hasEdge(keys.at(lf), keys.at(rg))) {
        if (lf == rg) {
          idx = 1 + 3 * lf;
          // self-loop
          diagram.at(rootIndex).at(idx) |= CellState::VERTEX | CellState::NORTH | CellState::WEST;
          diagram.at(rootIndex).at(idx - 1) |= CellState::NORTH | CellState::EAST;
          if (rootIndex == 0) {
            diagram.push_front(std::vector<CellState>(2 + 3 * keys.size(), CellState::NONE));
            ++rootIndex;
          }
          diagram.at(rootIndex - 1).at(idx) |= CellState::SOUTH | CellState::WEST;
          diagram.at(rootIndex - 1).at(idx - 1) |= CellState::SOUTH | CellState::EAST;
        } else {
          // other
        }
      }
    }
  }

  return std::string{};
}

} // namespace utl
