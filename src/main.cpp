#include <array>
#include <deque>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <format>

using strv = std::string_view;
using prefMap = std::multimap<size_t, strv, std::less<size_t>>;
using matchMap = std::map<strv, prefMap>;

void runTest();
void tempTest();

int main() {
  tempTest();
  runTest();
  return 0;
}

template <class K, class T>
void insertItem(K key, T value, std::map<K, T> &suppliers) {
  if (
    auto result = suppliers.insert({key, value}); !result.second
  ) {
    std::ostringstream msg;
    msg << "This function disallows repeated insertions. ";
    msg << "The key inserted was " << key;
    throw std::logic_error(msg.str());
  }
}

template <class K, class T, class I>
void insertPrefs(
  K key, std::map<K, T> &suppliers, const std::map<K, T> &consumers,
  std::initializer_list<std::pair<I, K>> prefs
) {
  if (suppliers.find(key) != suppliers.cend()) {
    for (auto const & pref: prefs) {
      if (consumers.find(pref.second) != consumers.cend()) {
        suppliers.at(key).insert(pref);
      } else {
        std::ostringstream msg;
        msg << "You are trying to link a supplier to a consumer ";
        msg << "that does not exist. The attempted key was: " << pref.second;
        throw std::logic_error(msg.str());
      }
    }
  } else {
    std::ostringstream msg;
    msg << "This function disallows repeated insertions. ";
    msg << "The key inserted was " << key;
    throw std::logic_error(msg.str());
  }
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

void prettyPrint(matchMap items) {
  for (auto const &item: items) {
    std::cout << std::format("{}: ", item.first);
    for (const auto &pref: item.second) {
      std::cout << std::format("{:>2},", pref.second);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void tempTest() {
  matchMap hospitals;
  matchMap students;

  insertItem<strv, prefMap>("A", prefMap{}, hospitals);
  insertItem<strv, prefMap>("B", prefMap{}, hospitals);
  insertItem<strv, prefMap>("C", prefMap{}, hospitals);

  insertItem<strv, prefMap>("X", prefMap{}, students);
  insertItem<strv, prefMap>("Y", prefMap{}, students);
  insertItem<strv, prefMap>("Z", prefMap{}, students);

  insertPrefs<strv, prefMap, size_t>(
    "A", hospitals, students, {{1, "X"}, {2, "Y"}, {3, "Z"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "B", hospitals, students, {{1, "Y"}, {2, "X"}, {3, "Z"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "C", hospitals, students, {{1, "X"}, {2, "Y"}, {3, "Z"}}
  );

  insertPrefs<strv, prefMap, size_t>(
    "X", students, hospitals, {{1, "B"}, {2, "A"}, {3, "C"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "Y", students, hospitals, {{1, "A"}, {2, "B"}, {3, "C"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "Z", students, hospitals, {{1, "A"}, {2, "B"}, {3, "C"}}
  );

  prettyPrint(hospitals);
  prettyPrint(students);
}

void runTest() {
  matchMap hospitals;
  matchMap students;

  insertItem<strv, prefMap>("A", prefMap{}, hospitals);
  insertItem<strv, prefMap>("B", prefMap{}, hospitals);
  insertItem<strv, prefMap>("C", prefMap{}, hospitals);
  insertItem<strv, prefMap>("D", prefMap{}, hospitals);

  insertItem<strv, prefMap>("X", prefMap{}, students);
  insertItem<strv, prefMap>("Y", prefMap{}, students);
  insertItem<strv, prefMap>("Z", prefMap{}, students);
  insertItem<strv, prefMap>("W", prefMap{}, students);

  insertPrefs<strv, prefMap, size_t>(
    "A", hospitals, students, {{1, "X"}, {2, "Y"}, {4, "Z"}, {3, "W"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "B", hospitals, students, {{4, "X"}, {1, "Y"}, {2, "Z"}, {3, "W"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "C", hospitals, students, {{3, "X"}, {1, "Y"}, {4, "Z"}, {2, "W"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "D", hospitals, students, {{2, "X"}, {3, "Y"}, {1, "Z"}, {4, "W"}}
  );

  insertPrefs<strv, prefMap, size_t>(
    "X", students, hospitals, {{1, "A"}, {3, "B"}, {4, "C"}, {2, "D"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "Y", students, hospitals, {{3, "A"}, {1, "B"}, {4, "C"}, {2, "D"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "Z", students, hospitals, {{3, "A"}, {1, "B"}, {4, "C"}, {2, "D"}}
  );
  insertPrefs<strv, prefMap, size_t>(
    "W", students, hospitals, {{3, "A"}, {2, "B"}, {1, "C"}, {4, "D"}}
  );

  prettyPrint(hospitals);
  prettyPrint(students);
}
