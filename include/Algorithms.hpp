#pragma once

#include "Graph.hpp"
#include <algorithm>
#include <cstddef> // size_t
#include <optional>
#include <stack>
#include <unordered_map>
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

      stack.push(node.first);
      graphState.emplace(node.first, QState::VISITED);

      while (!stack.empty()) {
        auto currKey = stack.top();

        bool noNeighbors = true;

        for (auto const &neighbor : graph.vertex(currKey)->second) {
          if (auto state = graphState.find(neighbor.first->key());
              state != graphState.end() && state->second == QState::EXPLORED) {
            continue;
          } else if (state != graphState.end() &&
                     state->second == QState::VISITED) {
            return {};
          } else {
            noNeighbors = false;
            stack.push(neighbor.first->key());
            graphState.emplace(stack.top(), QState::VISITED);
            break;
          }
        }

        if (noNeighbors) {
          graphState.at(stack.top()) = QState::EXPLORED;
          sortedNodes.push_back(stack.top());
          stack.pop();
        }
      }
    }
  }

  std::reverse(sortedNodes.begin(), sortedNodes.end());
  return sortedNodes;
}

} // namespace alg
