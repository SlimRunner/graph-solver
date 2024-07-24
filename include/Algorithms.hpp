#pragma once

#include "Graph.hpp"
#include <algorithm>
#include <cstddef> // size_t
#include <optional>
#include <stack>
#include <vector>

namespace alg {

template <class K, class T, class W>
std::optional<std::vector<K>> topoSort(Graph<K, T, W> graph) {
  enum class QState {
    UNVISITED,
    VISITED,
    EXPLORED,
  };

  std::vector<K> sortedNodes;
  std::unordered_map<K, QState> graphState;

  // for each vertex in graph
  for (auto const &node : graph) {
    // if neighbor is not visited add to stack
    if (graphState.find(node.first) == graphState.end()) {
      std::stack<K> stack;
      std::vector<K> staging;

      stack.push(node.first);

      while (!stack.empty()) {
        auto currKey = stack.top();
        stack.pop();

        graphState.emplace(currKey, QState::VISITED);

        for (auto const &neighbor : graph.vertex(currKey)->second) {
          if (auto state = graphState.find(neighbor.first->key());
              state != graphState.end() && state->second == QState::EXPLORED) {
            continue;
          } else if (state != graphState.end() &&
                     state->second == QState::VISITED) {
            return {};
          } else {
            stack.push(neighbor.first->key());
          }
        }

        staging.push_back(currKey);
      }

      // copy staged keys in the right order
      while (staging.size()) {
        graphState.at(staging.back()) = QState::EXPLORED;
        sortedNodes.push_back(staging.back());
        staging.pop_back();
      }
    }
  }

  std::reverse(sortedNodes.begin(), sortedNodes.end());
  return sortedNodes;
}

} // namespace alg
