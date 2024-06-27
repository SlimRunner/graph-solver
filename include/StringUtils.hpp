#pragma once

#include <string>
#include <vector>

std::string stringBetween(const std::string & src, char start, char end);
std::string stringBefore(const std::string & src, char end);
std::string stringAfter(const std::string & src, char start);

std::vector<int> parseInts(const std::string & src);
std::vector<int> parseInts(const std::string & src, char skipChar);
std::vector<long> parseLongs(const std::string & src);
std::vector<long> parseLongs(const std::string & src, char skipChar);
std::vector<long long> parseLLs(const std::string & src);
std::vector<long long> parseLLs(const std::string & src, char skipChar);

std::vector<std::string> parseWords(const std::string & src);
std::vector<std::string> parseWordNums(const std::string & src);
