#pragma once
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
#include <unordered_set>


namespace string_util {
  bool isAlphanumeric(const std::string& str);
  bool isAlphabetic(const std::string& str);
  bool isNumeric(const std::string& str);
  bool isHexadecimal(const std::string& str);

  bool isCharAlphanumeric(unsigned char c);
  bool isCharAlphabetic(unsigned char c);
  bool isCharNumeric(unsigned char c);
  bool isCharHexadecimal(unsigned char c);

  bool isFloat(const std::string& str);
  bool isInteger(const std::string& str);

  bool startsWith(std::string str, std::string start);
  bool startsWith(std::string str, std::function<bool(unsigned char)> filter, int count);
  bool endsWith(std::string str, std::string end);
  bool endsWith(std::string str, std::function<bool(unsigned char)> filter, int count);


  int findFirstOf(const std::string& input, const std::string& target);
  int findLastOf(const std::string& input, const std::string& target);

  int findFirstOf(const std::string& input, std::function<bool(unsigned char)> pattern);
  int findLastOf(const std::string& input, std::function<bool(unsigned char)> pattern);
  
  bool containsOnly(const std::string& input, std::function<bool(unsigned char)> restriction);
}
