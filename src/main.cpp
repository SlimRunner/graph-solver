#include "StableMatch.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

void runTestCases();

int main() {
  runTestCases();
  
  return 0;
}

void runTestCases() {
  const auto testFiles = std::filesystem::directory_iterator("./test-cases/");

  for (const auto &entry : testFiles) {
    if (
      entry.path().extension() == ".txt" &&
      entry.path().filename().string().find("input-") != std::string::npos
    ) {
      std::ifstream infile(entry.path());
      std::stringstream fstr;
      fstr << infile.rdbuf();
      std::cout << std::format(" {:=^80} \n", entry.path().filename().string());;

      alg::StableMatch sm(
        fstr.str()
      );
      std::cout << sm.toString();

      std::cout << "MATCHES: ";
      bool leadingTerm = true;
      for (auto const &m : sm.findMatches()) {
        std::cout << (leadingTerm ? "" : ", ");
        std::cout << m.second << "-" << m.first;
        leadingTerm = false;
      }
      std::cout << "\n" << std::endl;
    }
  }
}
