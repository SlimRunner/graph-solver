#include <iostream>

#include "StableMatch.hpp"

void runTest();
void runTest2();
void runTest3();
void testDefault();

int main() {
  testDefault();
  runTest();
  runTest2();
  runTest3();
  return 0;
}

void testDefault() {
  alg::StableMatch sm(
    "A: X,Y,Z;"
    "B: Y,X,Z;"
    "C: X,Y,Z;"
    ";"
    "X: B,A,C;"
    "Y: A,B,C;"
    "Z: A,B,C;"
  );
  std::cout << sm.toString();

  std::cout << "MATCHES: ";
  for (auto const &m: sm.findMatches()) {
    std::cout << m.first << "-" << m.second << ", ";
  }
  std::cout << "\n" << std::endl;
}

void runTest() {
  alg::StableMatch sm(
    "A: 1-X,2-Y,4-Z,3-W;"
    "B: 4-X,1-Y,2-Z,3-W;"
    "C: 3-X,1-Y,4-Z,2-W;"
    "D: 2-X,3-Y,1-Z,4-W;"
    ";"
    "X: 1-A,3-B,4-C,2-D;"
    "Y: 3-A,1-B,4-C,2-D;"
    "Z: 3-A,1-B,4-C,2-D;"
    "W: 3-A,2-B,1-C,4-D;",
    true
  );
  std::cout << sm.toString();
  
  std::cout << "MATCHES: ";
  for (auto const &m: sm.findMatches()) {
    std::cout << m.first << "-" << m.second << ", ";
  }
  std::cout << "\n" << std::endl;
}

void runTest2() {
  alg::StableMatch sm(
    "Emory: Xavier, Yolanda, Zeus;"
    "MGH: Yolanda, Xavier, Zeus;"
    "NorthW: Xavier, Yolanda, Zeus;"
    ";"
    "Xavier: MGH, Emory, NorthW;"
    "Yolanda: Emory, MGH, NorthW;"
    "Zeus: Emory, MGH, NorthW;"
  );
  std::cout << sm.toString();
  
  std::cout << "MATCHES: ";
  for (auto const &m: sm.findMatches()) {
    std::cout << m.first << "-" << m.second << ", ";
  }
  std::cout << "\n" << std::endl;
}

void runTest3() {
  alg::StableMatch sm(
    "Emory: Wayne, Val, Yolanda, Zeus, Xavier;"
    "MGH: Yolanda, Wayne, Val, Xavier, Zeus;"
    "NorthW: Wayne, Zeus, Xavier, Yolanda, Val;"
    "UMich: Val, Yolanda, Xavier, Wayne, Zeus;"
    "UTSouth: Wayne, Yolanda, Val, Zeus, Xavier;"
    ";"
    "Val: UTSouth, Emory, MGH, UMich, NorthW;"
    "Wayne: NorthW, MGH, UMich, Emory, UTSouth;"
    "Xavier: MGH, NorthW, UMich, UTSouth, Emory;"
    "Yolanda: Emory, UTSouth, UMich, NorthW, MGH;"
    "Zeus: UMich, MGH, UTSouth, NorthW, Emory;"
  );
  std::cout << sm.toString();
  
  std::cout << "MATCHES: ";
  for (auto const &m: sm.findMatches()) {
    std::cout << m.first << "-" << m.second << ", ";
  }
  std::cout << "\n" << std::endl;
}
