#include "UnitTests.hpp"
#include "Algorithms.hpp"
#include "StableMatch.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

namespace tst {

template <class T> static void printTime(T time) {
  std::cout << time / 1000;
  std::cout << " ms + ";
  std::cout << time % 1000;
  std::cout << " us\n" << std::endl;
}

void stableMatchStressTest(size_t entryCount) {
  using steady = std::chrono::steady_clock;
  using chrono_us = std::chrono::microseconds;
  using namespace std::chrono_literals;

  alg::StableMatch smParser;
  std::deque<std::string> hospitals;
  std::deque<std::string> students;
  std::ostringstream oss;
  const size_t numWidth =
      static_cast<size_t>(std::ceil(std::log10(entryCount)));

  std::cout << "== Stress Test =============================\n";
  std::cout << "Entry allocation started..." << std::endl;
  steady::time_point beginAlloc = steady::now();
  for (size_t i = 0; i < entryCount; ++i) {
    oss << std::setw(numWidth) << std::setfill('0') << std::right << i;
    hospitals.push_back("H" + oss.str());
    smParser.insertProvider(hospitals.back());
    students.push_back("s" + oss.str());
    smParser.insertConsumer(students.back());
    oss.str("");
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rng(seed);

  for (const auto &h : hospitals) {
    std::shuffle(students.begin(), students.end(), rng);
    size_t prio = 0;
    for (const auto &s : students) {
      smParser.addProviderPreference(h, {s, prio++});
    }
  }
  for (const auto &s : students) {
    std::shuffle(hospitals.begin(), hospitals.end(), rng);
    size_t prio = 0;
    for (const auto &h : hospitals) {
      smParser.addConsumerPreference(s, {h, prio++});
    }
  }
  steady::time_point endAlloc = steady::now();
  auto allocTimeDelta =
      std::chrono::duration_cast<chrono_us>(endAlloc - beginAlloc).count();
  std::cout << "allocation in ";
  printTime(allocTimeDelta);

  steady::time_point beginAlgo = steady::now();
  const auto stableMatches = smParser.findMatches();
  steady::time_point endAlgo = steady::now();

  auto algoTimeDelta =
      std::chrono::duration_cast<chrono_us>(endAlgo - beginAlgo).count();
  auto printMatches = [stableMatches]() {
    std::cout << "Stable matches (" << stableMatches.size() << ")" << ": \n";
    bool leadingTerm = true;
    for (auto const &sm : stableMatches) {
      std::cout << (leadingTerm ? "" : ", ");
      std::cout << sm.second << "-" << sm.first;
      leadingTerm = false;
    }
    std::cout << "\n" << std::endl;
  };

  if (stableMatches.size() > 500) {
    std::cout << "entries are too many to print\n\n";
    std::cout << "matches are too many to print";
    std::cout << "\n" << std::endl;
  } else if (entryCount > 30) {
    std::cout << "entries are too many to print";
    std::cout << "\n" << std::endl;
    printMatches();
  } else {
    std::cout << smParser.toString();
    printMatches();
  }

  std::cout << entryCount;
  std::cout << " entries in ";
  printTime(algoTimeDelta);
}

void stableMatchCases() {
  using steady = std::chrono::steady_clock;
  using chrono_us = std::chrono::microseconds;

  const auto testFiles = std::filesystem::directory_iterator("./test-cases/");

  for (const auto &entry : testFiles) {
    if (entry.path().extension() == ".txt" &&
        entry.path().filename().string().find("sm-input-") !=
            std::string::npos) {
      std::ifstream infile(entry.path());
      std::stringstream fstr;

      fstr << infile.rdbuf();
      std::cout << "== " << entry.path().filename().string()
                << " =============================\n";

      alg::StableMatch smParser(fstr.str());
      std::cout << smParser.toString();

      steady::time_point begin = steady::now();
      const auto stableMatches = smParser.findMatches();
      steady::time_point end = steady::now();

      std::cout << "Stable matches: \n";
      bool leadingTerm = true;
      for (auto const &sm : stableMatches) {
        std::cout << (leadingTerm ? "" : ", ");
        std::cout << sm.second << "-" << sm.first;
        leadingTerm = false;
      }
      std::cout << "\n" << std::endl;
      auto timeDelta =
          std::chrono::duration_cast<chrono_us>(end - begin).count();

      std::cout << stableMatches.size();
      std::cout << " entries in ";
      printTime(timeDelta);
    }
  }
}

void topoSortSimpleTest() {
  auto printOutput = [](std::vector<std::string> values) {
    std::cout << "sorted: ";
    for (auto const &v : values) {
      std::cout << v << ", ";
    }
    std::cout << "\n" << std::endl;
  };

  const auto testFiles = std::filesystem::directory_iterator("./test-cases/");

  for (const auto &entry : testFiles) {
    if (entry.path().extension() == ".txt" &&
        entry.path().filename().string().find("grph-input") !=
            std::string::npos) {
      std::ifstream infile(entry.path());
      std::stringstream fstr;

      fstr << infile.rdbuf();
      std::cout << "== " << entry.path().filename().string()
                << " =============================\n";

      alg::Graph<std::string, int, int> G(fstr.str());
      std::cout << G.toString();
      auto sorted = alg::topoSort(G);

      if (sorted.has_value()) {
        printOutput(sorted.value());
        auto strG = G.toDiagram(sorted.value().begin(), sorted.value().end());
        if (strG.has_value()) {
          std::cout << strG.value() << std::endl;
        }
      } else {
        std::cout << "has cycle\n" << std::endl;
      }
    }
  }
}

} // namespace tst
