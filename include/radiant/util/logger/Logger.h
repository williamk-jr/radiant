#pragma once
#include "radiant/util/logger/MessageStyle.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <string>
#include <chrono>

namespace Radiant {
  struct LogPrefix;

  class Logger {
    public: 
      static void info(const std::string& message);
      static void info(const std::string& message, unsigned short verbosity);
      static void warn(const std::string& message);
      static void error(const std::string& message);
      static void fatal(const std::string& message);

      static void info(const std::string& message, std::vector<LogPrefix> prefixes);
      static void info(const std::string& message, std::vector<LogPrefix> prefixes, unsigned short verbosity);
      static void warn(const std::string& message, std::vector<LogPrefix> prefixes);
      static void error(const std::string& message, std::vector<LogPrefix> prefixes);
      static void fatal(const std::string& message, std::vector<LogPrefix> prefixes);

      static void shouldAllowExceptions(bool value);
      static void shouldAllowColor(bool value);
      static void setVerbosity(unsigned short verbosity);

      static bool canThrowExceptions();
      static bool canShowColor();
      static unsigned short getVerbosity();
    private:
      static bool allowExceptions;
      static bool allowColor;
      static unsigned short verbosity;

      static std::string formatAll(std::vector<LogPrefix> prefixes);
  };

  struct LogPrefix {
    std::string message;
    MessageStyle::MessageColor color;

    std::string format() {
      if (Logger::canShowColor()) {
        return std::string(color)+"["+message+"]"+"\x1b[0m";
      }
      return "["+message+"]";
    }
  };

  namespace LogPrefixes {
    LogPrefix time(MessageStyle::MessageColor color);
  }
}
