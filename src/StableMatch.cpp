#include "StableMatch.hpp"

#include "StringUtils.hpp"
#include <deque>
#include <exception>
#include <queue>
#include <sstream>
#include <vector>

namespace alg {

static void insertItem(strv key, matchMap &items) {
  if (auto result = items.insert({key, prefMap{}}); !result.second) {
    std::ostringstream msg;
    msg << "This function disallows repeated insertions. ";
    msg << "The key inserted was " << key;
    throw std::logic_error(msg.str());
  }
}

static void insertPref(strv key, matchMap &judges, const matchMap &candidates,
                       prefPairs pref) {
  if (judges.find(key) != judges.cend()) {
    if (candidates.find(pref.first) != candidates.cend()) {
      judges.at(key).insert(pref);
    } else {
      std::ostringstream msg;
      msg << "You are trying to link a supplier to a consumer that does ";
      msg << "not exist. The attempted candidate is `" << pref.first;
      msg << "` and the judge is " << key;
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

StableMatch::StableMatch(std::string str, bool explicitPriority)
    : mProviders{}, mConsumers{} {
  using namespace sutl;
  bool swapTarget = false;
  constexpr auto EXPL_WORD = "explicit";
  constexpr auto EXPL_SIZE = sizeof EXPL_WORD;

  if (auto pos = str.find(EXPL_WORD); pos != std::string::npos) {
    str = str.replace(pos, EXPL_SIZE, "");
    explicitPriority = true;
  }

  auto const inputLines = split(str, ';');

  for (const auto &lineRaw : inputLines) {
    const auto &line = trim(lineRaw);
    if (line.length() == 0)
      continue;
    if (line.length() == 3 && line == "***") {
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
  for (const auto &lineRaw : inputLines) {
    const auto &line = trim(lineRaw);
    if (line.length() == 3 && line == "***") {
      swapTarget = true;
    } else if (!swapTarget) {
      const auto currKey = trim(stringBefore(line, ':'));
      int priority = 0; // lower means more priority
      for (const auto &pref : split(stringAfter(line, ':'), ',')) {
        if (!explicitPriority) {
          addProviderPreference(currKey, {trim(pref), priority++});
        } else {
          const auto kvPair = split(pref, '-');
          addProviderPreference(
              currKey, {trim(kvPair.at(1)), std::stoi(trim(kvPair.at(0)))});
        }
      }
    } else {
      const auto currKey = trim(stringBefore(line, ':'));
      int priority = 0; // lower means more priority
      for (const auto &pref : split(stringAfter(line, ':'), ',')) {
        if (!explicitPriority) {
          addConsumerPreference(currKey, {trim(pref), priority++});
        } else {
          const auto kvPair = split(pref, '-');
          addConsumerPreference(
              currKey, {trim(kvPair.at(1)), std::stoi(trim(kvPair.at(0)))});
        }
      }
    }
  }
}

void StableMatch::insertProvider(strv key) { insertItem(key, mProviders); }

void StableMatch::insertConsumer(strv key) { insertItem(key, mConsumers); }

void StableMatch::addProviderPreference(strv key, prefPairs preferences) {
  insertPref(key, mProviders, mConsumers, preferences);
}

void StableMatch::addConsumerPreference(strv key, prefPairs preferences) {
  insertPref(key, mConsumers, mProviders, preferences);
}

std::string StableMatch::toString() const {
  std::ostringstream ss;

  for (const auto &items : {mProviders, mConsumers}) {
    for (auto const &item : items) {
      ss << item.first << ": ";

      // this is done to sort by preference
      prefMapInv sortedPrefs;
      for (auto const &p : item.second) {
        sortedPrefs.insert({p.second, p.first});
      }

      bool leadingTerm = true;
      for (const auto &pref : sortedPrefs) {
        ss << (leadingTerm ? "" : ", ") << pref.second;
        leadingTerm = false;
      }
      ss << '\n';
    }
    ss << '\n';
  }
  return ss.str();
}

std::map<strv, strv> StableMatch::findMatches() const {
  using iterMapInv = std::map<strv, prefMapInv::const_iterator>;
  iterMapInv state;
  std::queue<iterMapInv::iterator> queue;
  std::map<strv, strv> matches;
  std::vector<prefMapInv> sortedPrefs;

  // set up state of each provider
  for (auto const &mProv : mProviders) {
    sortedPrefs.push_back({});
    for (auto const &p : mProv.second) {
      sortedPrefs.back().insert({p.second, p.first});
    }

    state.insert({mProv.first, sortedPrefs.back().begin()});
  }

  // fill queue with all providers
  for (auto it = state.begin(); it != state.end(); ++it) {
    queue.push(it);
  }

  for (; !queue.empty();) {
    auto const providerHash = queue.front()->first;
    auto const candidateHash = queue.front()->second->second;
    auto const candidatePref = mConsumers.at(candidateHash);
    if (matches.find(candidateHash) == matches.end()) {
      matches.insert({candidateHash, providerHash});
      queue.pop();
    } else if (auto &currMatchHash = matches.at(candidateHash);
               candidatePref.at(providerHash) <
               candidatePref.at(currMatchHash)) {
      ++state.at(currMatchHash);
      queue.push(state.find(currMatchHash));
      currMatchHash = providerHash;
      queue.pop();
    } else {
      ++state.at(providerHash);
    }
  }

  return matches;
}

} // namespace alg
