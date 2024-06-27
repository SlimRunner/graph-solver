#include "StringUtils.hpp"

#include <cctype>
#include <cstddef> // size_t
#include <sstream>

using funcInt2Int = int (*)(int);
using funcInt2IntNoEx = int (*)(int) noexcept(true);

template <class T>
using funcStringParse = T (*)(const std::string &, size_t *, int);

#if defined _WIN64 || defined _WIN32
static funcInt2IntNoEx exceptify(funcInt2Int discriminator) {
  return reinterpret_cast<funcInt2IntNoEx>(discriminator);
}
#elif defined __linux__
static funcInt2IntNoEx exceptify(funcInt2IntNoEx discriminator) {
  return discriminator;
}
#endif

std::string stringBetween(const std::string &src, char start, char end) {
  size_t p0, p1;
  p0 = src.find(start);
  p1 = src.find(end);
  if (p0++ == src.npos || p1 == src.npos || p0 >= p1) {
    return std::string{};
  } else {
    return src.substr(p0, p1 - p0);
  }
}

std::string stringBefore(const std::string & src, char end) {
  size_t p1;
  p1 = src.find(end);
  if (p1 == src.npos || p1 <= 0) {
    return std::string{};
  } else {
    return src.substr(0, p1);
  }
}

std::string stringAfter(const std::string & src, char start) {
  size_t p0;
  p0 = src.find(start);
  if (p0++ == src.npos || p0 >= src.length()) {
    return std::string{};
  } else {
    return src.substr(p0);
  }
}

template <class T>
std::vector<T> parseChars(const std::string & src, funcInt2IntNoEx discriminator) {
  std::vector<T> result;
  std::stringstream buffer;
  bool enableBuffer = false;
  
  for (auto const & chr: src) {
    if (discriminator(static_cast<unsigned char>(chr))) {
      buffer << chr;
      enableBuffer = true;
    } else if (enableBuffer) {
      result.push_back(buffer.str());
      buffer.clear();
      buffer.str("");
      enableBuffer = false;
    }
  }

  if (enableBuffer) {
    result.push_back(buffer.str());
  }

  return result;
}

template <class T>
std::vector<T> parseNums(const std::string & src, funcStringParse<T> parser) {
  std::vector<T> result;
  std::stringstream buffer;
  bool enableBuffer = false;

  for (auto const & chr: src) {
    if (std::isdigit(chr)) {
      buffer << chr;
      enableBuffer = true;
    } else if (enableBuffer) {
      result.push_back(parser(buffer.str(), (size_t *)nullptr, 10));
      buffer.clear();
      buffer.str("");
      enableBuffer = false;
    } else if (chr == '-') {
      buffer << chr;
      enableBuffer = true;
    }
  }

  if (enableBuffer) {
    result.push_back(parser(buffer.str(), (size_t *)nullptr, 10));
  }

  return result;
}

template <class T>
std::vector<T> parseNums(const std::string & src, char skipChar, funcStringParse<T> parser) {
  std::vector<T> result;
  std::stringstream buffer;
  bool enableBuffer = false;
  
  for (auto const & chr: src) {
    if (std::isdigit(chr)) {
      buffer << chr;
      enableBuffer = true;
    } else if (chr != skipChar && enableBuffer) {
      result.push_back(parser(buffer.str(), (size_t *)nullptr, 10));
      buffer.clear();
      buffer.str("");
      enableBuffer = false;
    } else if (chr != skipChar && chr == '-') {
      buffer << chr;
      enableBuffer = true;
    }
  }

  if (enableBuffer) {
    result.push_back(parser(buffer.str(), (size_t *)nullptr, 10));
  }

  return result;
}

std::vector<int> parseInts(const std::string & src) {
  return parseNums(src, std::stoi);
}

std::vector<int> parseInts(const std::string & src, char skipChar) {
  return parseNums(src, skipChar, std::stoi);
}

std::vector<long> parseLongs(const std::string & src) {
  return parseNums(src, std::stol);
}

std::vector<long> parseLongs(const std::string & src, char skipChar) {
  return parseNums(src, skipChar, std::stol);
}

std::vector<long long> parseLLs(const std::string & src) {
  return parseNums(src, std::stoll);
}

std::vector<long long> parseLLs(const std::string & src, char skipChar) {
  return parseNums(src, skipChar, std::stoll);
}

std::vector<std::string> parseWords(const std::string &src) {
  return parseChars<std::string>(src, exceptify(std::isalpha));
}

std::vector<std::string> parseWordNums(const std::string &src) {
  return parseChars<std::string>(src, exceptify(std::isalnum));
}
