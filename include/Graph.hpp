#pragma once

#include "Node.hpp"
#include <cstddef> // size_t
#include <optional>
#include <unordered_map>

namespace alg {

template <class K, class T, class W> class Graph {
private:
  using vtxHash = std::unordered_map<K, Node<K, T, W>>;

  vtxHash mVertices;

public:
  Graph() : mVertices{} {}
  template <class It> Graph(It begin, It end) : Graph{} {
    std::copy(begin, end, std::inserter(mVertices, mVertices.end()));
  }

  bool addNode(K key, T value) {
    auto result = mVertices.emplace(key, Node<K, T, W>{key, value});
    return result.second;
  }

  bool addEdge(K from, K to, W weight = 0) {
    auto vit = mVertices.find(from);
    auto wit = mVertices.find(to);

    if (vit == mVertices.end() || wit == mVertices.end()) {
      return false;
    }

    return vit->second.connectWith(&(wit->second), weight);
  }

  inline vtxHash::const_iterator vertex(K key) const {
    return mVertices.find(key);
  }
};

} // namespace alg
