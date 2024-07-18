#include "UnitTests.hpp"
#include <iostream>

int main() {
  tst::stableMatchCases();
  tst::stableMatchStressTest(2000);

  // profiling results 2024-06-30:
  // https://www.desmos.com/calculator/nrzic3beq3
  // data seems ambiguous between O(n log n) and O(n^2)
  // static analisys suggests it should be O(n log n)
  // but I might be doing it wrong.
  return 0;
}


