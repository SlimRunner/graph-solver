#pragma once

#include <algorithm> // copy, make_heap
#include <cstddef>   // size_t
#include <unordered_map>

namespace alg {

template <class K, class T, class W> class Node {
private:
  using adjHash = std::unordered_map<Node *, W>;

  adjHash mAdjList;
  T mValue;
  const K mKey;
  Node() : mAdjList{}, mValue{} {}

public:
  Node(K key) : mAdjList{}, mValue{}, mKey{key} {}
  Node(K key, T value) : mAdjList{}, mValue{value}, mKey{key} {}

  inline bool connectWith(Node *vertex, W weight = 0) {
    auto result = mAdjList.emplace(vertex, weight);
    return result.second;
  }

  inline adjHash::iterator begin() { return mAdjList.begin(); }

  inline adjHash::iterator end() { return mAdjList.end(); }

  inline const K &key() const { return mKey; }
};

} // namespace alg
