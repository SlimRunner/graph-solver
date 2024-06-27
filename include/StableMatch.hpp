#pragma once

#include <cstddef> // size_t
#include <initializer_list>
#include <map>
#include <string>

namespace alg {

using strv = std::string;
using prefMap = std::multimap<size_t, strv, std::less<size_t>>;
using matchMap = std::map<strv, prefMap>;
using prefPairs = std::pair<size_t, strv>;

class StableMatch {
private:
  matchMap mProviders;
  matchMap mConsumers;

public:
  StableMatch();
  StableMatch(std::string str, bool explicitPriority = false);

  void insertProvider(strv key);
  void insertConsumer(strv key);

  void addProviderPreference(strv key, prefPairs preference);
  void addConsumerPreference(strv key, prefPairs preference);

  std::string toString() const;
};

} // namespace alg