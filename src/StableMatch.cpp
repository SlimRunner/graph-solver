#include "StableMatch.hpp"

#include "StringUtils.hpp"
#include <deque>
#include <exception>
#include <format>
#include <queue>
#include <sstream>
#include <vector>

namespace alg {
  
static void insertItem(strv key, matchMap &items) {
  if (
    auto result = items.insert({key, prefMap{}}); !result.second
  ) {
    std::ostringstream msg;
    msg << "This function disallows repeated insertions. ";
    msg << "The key inserted was " << key;
    throw std::logic_error(msg.str());
  }
}

static void insertPref(
  strv key, matchMap &judges, const matchMap &candidates,
  prefPairs pref
) {
  if (judges.find(key) != judges.cend()) {
    if (candidates.find(pref.first) != candidates.cend()) {
      judges.at(key).insert(pref);
    } else {
      std::ostringstream msg;
      msg << "You are trying to link a supplier to a consumer ";
      msg << "that does not exist. The attempted key was: " << pref.first;
      throw std::logic_error(msg.str());
    }
  } else {
    std::ostringstream msg;
    msg << "This function disallows repeated insertions. ";
    msg << "The key inserted was " << key;
    throw std::logic_error(msg.str());
  }
}

StableMatch::StableMatch() : mProviders{}, mConsumers{} {}

StableMatch::StableMatch(std::string str, bool explicitPriority) : mProviders{}, mConsumers{} {
  bool swapTarget = false;
  for (const auto &line: split(std::string{str}, ';')) {
    if (line.length() == 0) {
      swapTarget = true;
    } else if (!swapTarget) {
      const auto currKey = trim(stringBefore(line, ':'));
      insertProvider(currKey);
    } else {
      const auto currKey = trim(stringBefore(line, ':'));
      insertConsumer(currKey);
    }
  }
  swapTarget = false;
  for (const auto &line: split(std::string{str}, ';')) {
    if (line.length() == 0) {
      swapTarget = true;
    } else if (!swapTarget) {
      const auto currKey = trim(stringBefore(line, ':'));
      int priority = 0; // lower means more priority
      for (const auto &pref: split(stringAfter(line, ':'), ',')) {
        if (!explicitPriority) {
          addProviderPreference(currKey, {trim(pref), priority++});
        } else {
          const auto kvPair = split(pref, '-');
          addProviderPreference(currKey, {trim(kvPair.at(1)), std::stoi(trim(kvPair.at(0)))});
        }
      }
    } else {
      const auto currKey = trim(stringBefore(line, ':'));
      int priority = 0; // lower means more priority
      for (const auto &pref: split(stringAfter(line, ':'), ',')) {
        if (!explicitPriority) {
          addConsumerPreference(currKey, {trim(pref), priority++});
        } else {
          const auto kvPair = split(pref, '-');
          addConsumerPreference(currKey, {trim(kvPair.at(1)), std::stoi(trim(kvPair.at(0)))});
        }
      }
    }
  }
}

void StableMatch::insertProvider(strv key) {
  insertItem(key, mProviders);
}

void StableMatch::insertConsumer(strv key) {
  insertItem(key, mConsumers);
}

void StableMatch::addProviderPreference(strv key, prefPairs preferences) {
  insertPref(key, mProviders, mConsumers, preferences);
}

void StableMatch::addConsumerPreference(strv key, prefPairs preferences) {
  insertPref(key, mConsumers, mProviders, preferences);
}

std::string StableMatch::toString() const {
  std::ostringstream ss;

  for (const auto & items: {mProviders, mConsumers}) {
    for (auto const &item: items) {
      ss << std::format("{}: ", item.first);

      // this is done to sort by preference
      prefMapInv sortedPrefs;
      for (auto const &p: item.second) {
        sortedPrefs.insert({p.second, p.first});
      }

      bool leadingTerm = true;
      for (const auto &pref: sortedPrefs) {
        ss << (
          leadingTerm ?
          std::format(  "{}", pref.second) :
          std::format(", {}", pref.second)
        );
        leadingTerm = false;
      }
      ss << '\n';
    }
    ss << '\n';
  }
  return ss.str();
}

} // namespace alg
