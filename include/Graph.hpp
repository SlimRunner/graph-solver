#pragma once

#include "Node.hpp"
#include "StringUtils.hpp"
#include <cstddef> // size_t
#include <deque>
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
    for (auto &&line : inputLines) {
      auto key = trim(stringBefore(line, ':'));
      // value of node is defaulted to 0 for now
      // better parsing is a WIP
      addNode(key, T{});
    }

    for (const auto &line : inputLines) {
      auto key = trim(stringBefore(line, ':'));
      auto rels = trim(stringAfter(line, ':'));
      for (const auto &node : split(rels, ',')) {
        addEdge(key, trim(node));
      }
    }
  }

  bool addNode(K key, T value) {
    auto result = mVertices.emplace(key, Node<K, T, W>{key, value});
    return result.second;
  }

  bool addEdge(K from, K to, W weight = W{}) {
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

  inline bool hasNode(K n) { return mVertices.find(n) != mVertices.end(); }

  inline bool hasEdge(K n1, K n2) {
    auto const vtx1 = mVertices.find(n1);
    auto const vtx2 = mVertices.find(n2);
    return (vtx1 != mVertices.end() && vtx2 != mVertices.end() &&
            vtx1->second.adjacentTo(&(vtx2->second)) != vtx1->second.end());
  }

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

  // THIS FUNCTION IS AN STALE WORK IN PROGRESS
  template <class It> std::optional<std::string> toDiagram(It begin, It end) {
    std::vector<K> keys(begin, end);
    std::vector<std::string> keyText;

    // for now this function can only print DAGs

    for (auto const &k : keys) {
      if (!hasNode(k)) {
        return {};
      }
      std::ostringstream oss;
      oss << k;
      keyText.push_back(oss.str());
    }

    std::deque<std::ostringstream> diagram;
    size_t rootIndex = 0;
    diagram.push_back({});
    
    for (size_t lf = 0, rg = 1; rg < keys.size(); ++lf, ++rg) {
      diagram.at(rootIndex) << keyText.at(lf);
      if (hasEdge(keys.at(lf), keys.at(rg))) {
        diagram.at(rootIndex) << "->-";
      } else {
        diagram.at(rootIndex) << "   ";
      }
    }
    diagram.at(rootIndex) << keyText.back();

    return diagram.at(rootIndex).str();
  }
};

} // namespace alg
