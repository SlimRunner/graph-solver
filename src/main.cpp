#include "StableMatch.hpp"
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

void runTestCases();

int main() {
  runTestCases();

  return 0;
}

void runTestCases() {
  using steady = std::chrono::steady_clock;
  using chrono_ms = std::chrono::microseconds;
  using namespace std::chrono_literals;

  const auto testFiles = std::filesystem::directory_iterator("./test-cases/");

  for (const auto &entry : testFiles) {
    if (entry.path().extension() == ".txt" &&
        entry.path().filename().string().find("input-") != std::string::npos) {
      std::ifstream infile(entry.path());
      std::stringstream fstr;

      fstr << infile.rdbuf();
      std::cout << std::format("{:=^80}\n", entry.path().filename().string());

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
      
      if ((end - begin) > 1ms) {
        std::cout << "it took ";
        std::cout << std::chrono::duration_cast<chrono_ms>(end - begin).count() / 1000;
        std::cout << " ms" << std::endl;
        // timing results 2024-06-28: https://www.desmos.com/calculator/9zaanojuio
        // data suggests time complexity is O(n log n) just like predicted
      }
    }
  }
}
