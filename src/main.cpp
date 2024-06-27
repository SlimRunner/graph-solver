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

void runTest();

int main() {
  runTest();

  // std::queue<std::string, std::list<std::string>> hospitals;
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

void runTest() {
  using strv = std::string_view;
  using prefmap = std::multimap<size_t, strv, std::greater<size_t>>;

  std::map<strv, prefmap> hospitals;
  std::map<strv, prefmap> students;

  insertItem<strv, prefmap>("A", prefmap{}, hospitals);
  insertItem<strv, prefmap>("B", prefmap{}, hospitals);
  insertItem<strv, prefmap>("C", prefmap{}, hospitals);
  insertItem<strv, prefmap>("D", prefmap{}, hospitals);

  insertItem<strv, prefmap>("X", prefmap{}, students);
  insertItem<strv, prefmap>("Y", prefmap{}, students);
  insertItem<strv, prefmap>("Z", prefmap{}, students);
  insertItem<strv, prefmap>("W", prefmap{}, students);

  insertPrefs<strv, prefmap, size_t>(
    "A", hospitals, students, {{1, "X"}, {2, "Y"}, {4, "Z"}, {3, "W"}}
  );
  insertPrefs<strv, prefmap, size_t>(
    "B", hospitals, students, {{4, "X"}, {1, "Y"}, {2, "Z"}, {3, "W"}}
  );
  insertPrefs<strv, prefmap, size_t>(
    "C", hospitals, students, {{3, "X"}, {1, "Y"}, {4, "Z"}, {2, "W"}}
  );
  insertPrefs<strv, prefmap, size_t>(
    "D", hospitals, students, {{2, "X"}, {3, "Y"}, {1, "Z"}, {4, "W"}}
  );

  insertPrefs<strv, prefmap, size_t>(
    "X", students, hospitals, {{1, "A"}, {3, "B"}, {4, "C"}, {2, "D"}}
  );
  insertPrefs<strv, prefmap, size_t>(
    "Y", students, hospitals, {{3, "A"}, {1, "B"}, {4, "C"}, {2, "D"}}
  );
  insertPrefs<strv, prefmap, size_t>(
    "Z", students, hospitals, {{3, "A"}, {1, "B"}, {4, "C"}, {2, "D"}}
  );
  insertPrefs<strv, prefmap, size_t>(
    "W", students, hospitals, {{3, "A"}, {2, "B"}, {1, "C"}, {4, "D"}}
  );
}
