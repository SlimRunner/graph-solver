#pragma once

#include <string>
#include <string_view>
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

std::vector<std::string> split(const std::string & src, char delimiter);
std::vector<std::string> parseWords(const std::string & src);
std::vector<std::string> parseWordNums(const std::string & src);

std::string ltrim(std::string_view src);
std::string rtrim(std::string_view src);
std::string trim(std::string_view src);
