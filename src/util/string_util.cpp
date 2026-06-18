#include "radiant/util/string_util.h"
#include <algorithm>
#include <cctype>

namespace string_util {
  
  bool isAlphanumeric(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
      return std::isalnum(c);
    });
  }
  
  bool isAlphabetic(const std::string& str) { 
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
      return std::isalpha(c);
    });
  }

  bool isNumeric(const std::string& str) {
    int decimalCount = 0;
    return std::all_of(str.begin(), str.end(), [&decimalCount](unsigned char c) {
      if (c == '.') {
        decimalCount++;
        return (decimalCount <= 1);
      }
      return std::isdigit(c) != 0;
    }); 
  }

  bool isHexadecimal(const std::string& str) {
    return startsWith(str, "#") && std::all_of(str.begin()+1, str.end(), [](unsigned char c) {
      return isCharHexadecimal(c);
    });
  }

  bool isCharAlphanumeric(unsigned char c) {
    return std::isalnum(c);
  }

  bool isCharAlphabetic(unsigned char c) {
    return std::isalpha(c);
  }

  bool isCharNumeric(unsigned char c) {
    return std::isdigit(c);
  }

  bool isCharHexadecimal(unsigned char c) {
    return std::isxdigit(c); 
  }

  bool isFloat(const std::string& str) { 
    int decimalCount = 0;
    return std::all_of(str.begin(), str.end(), [&decimalCount](unsigned char c) {
      if (c == '.') {
        decimalCount++;
        return (decimalCount <= 1);
      }
      return std::isdigit(c) > 0;
    }) && decimalCount == 1; 
  }

  bool isInteger(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
      return std::isdigit(c) > 0;
    });
  }

  bool startsWith(std::string str, std::string start) {
    if (str.size() < start.size()) {
      return false;
    }

    return str.substr(0, start.size()) == start;
  }

  bool startsWith(std::string str, std::function<bool(unsigned char)> filter, int count) {
    return std::all_of(str.begin(), str.begin()+count, filter);
  }

  bool endsWith(std::string str, std::string end) {
    if (str.size() < end.size()) {
      return false;
    }

    return str.substr(str.size()-end.size(), str.size()) == end;
  }
 
  bool endsWith(std::string str, std::function<bool(unsigned char)> filter, int count) {
    return std::all_of(str.end()-count, str.end(), filter);
  }

  int findFirstOf(const std::string& input, const std::string& target) {
    for (int i = 0; i < input.size(); i++) {
      if (i+target.size() < input.size() && input.substr(i, target.size()) == target) {
        return i;
      }
    }

    return -1;
  }

  int findLastOf(const std::string& input, const std::string& target) {
    for (int i = input.size()-1; i >= 0; i--) {
      if (i+target.size() < input.size() && input.substr(i, target.size()) == target) {
        return i;
      }
    }

    return -1;
  }

  int findFirstOf(const std::string& input, std::function<bool(unsigned char)> pattern) {
    for (int i = 0; i < input.size(); i++) {
      if (pattern(input.at(i))) {
        return i;
      }
    }
    return -1;
  }

  int findLastOf(const std::string& input, std::function<bool(unsigned char)> pattern) {
    for (int i = input.size()-1; i >= 0; i--) {
      if (pattern(input.at(i))) {
        return i;
      }
    }
    return -1;
  }
  
  bool containsOnly(const std::string& input, std::function<bool(unsigned char)> comparison) {
    return std::all_of(input.begin(), input.end(), comparison);
  }
}
