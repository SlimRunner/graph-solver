#pragma once

#include "Node.hpp"
#include "StringUtils.hpp"
#include <cstddef> // size_t
#include <optional>
#include <sstream>
#include <string>
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

  Graph(std::string str) : Graph{} {
    using namespace sutl;

    auto inputLines = split(str, '\n');
    for (auto && line: inputLines) {
      auto key = trim(stringBefore(line, ':'));
      // value of node is defaulted to 0 for now
      // better parsing is a WIP
      addNode(key, T{});
    }

    for (const auto &line : inputLines) {
      auto key = trim(stringBefore(line, ':'));
      auto rels = trim(stringAfter(line, ':'));
      for (const auto &node: parseWordNums(rels)) {
        addEdge(key, node);
      }
    }
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

  inline vtxHash::iterator begin() { return mVertices.begin(); }

  inline vtxHash::iterator end() { return mVertices.end(); }

  inline vtxHash::const_iterator vertex(K key) const {
    return mVertices.find(key);
  }

  inline vtxHash::iterator vertex(K key) { return mVertices.find(key); }

  std::string toString() {
    std::ostringstream oss;
    for (auto &&u : *this) {
      oss << u.first << ": ";
      for (auto &&v : u.second) {
        oss << v.first->key() << ", ";
      }
      oss << std::endl;
    }
    oss << std::endl;
    return oss.str();
  }
};

} // namespace alg
