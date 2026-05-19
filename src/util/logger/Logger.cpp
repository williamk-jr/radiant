#include "radiant/util/logger/Logger.h"
#include <stdexcept>


namespace Radiant {

  namespace LogPrefixes {
    LogPrefix time(MessageStyle::MessageColor color) {
      std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::stringstream ss;
      ss << std::put_time(std::localtime(&now), "%F %T");
      return {ss.str(), color};
    }
  }

  bool Logger::allowExceptions = false;
  bool Logger::allowColor = false;
  unsigned short Logger::verbosity = 0;

  void Logger::info(const std::string& message) {
    Logger::info(message, {});
  }

  void Logger::info(const std::string& message, unsigned short verbosity) {
    Logger::info(message, {}, verbosity);
  }

  void Logger::warn(const std::string& message) {
    Logger::warn(message, {});
  }

  void Logger::error(const std::string& message) {
    Logger::error(message, {});
  }

  void Logger::fatal(const std::string& message) {
    Logger::fatal(message, {});
  }

  void Logger::info(const std::string& message, std::vector<LogPrefix> prefixes, unsigned short verbosity) {
    if (verbosity <= Logger::verbosity) {
      std::cout << LogPrefix{"INFO", MessageStyle::GREEN}.format() << Logger::formatAll(prefixes) << message << "\n";
    }
  }

  void Logger::info(const std::string& message, std::vector<LogPrefix> prefixes) {
    Logger::info(message, prefixes, 0);
  }

  void Logger::warn(const std::string& message, std::vector<LogPrefix> prefixes) {
    std::cout << LogPrefix{"WARN", MessageStyle::YELLOW}.format() << Logger::formatAll(prefixes) << message << "\n";
  }

  void Logger::error(const std::string& message, std::vector<LogPrefix> prefixes) {
    std::cerr << LogPrefix{"ERROR", MessageStyle::RED}.format() << Logger::formatAll(prefixes) << message << "\n";
  }

  void Logger::fatal(const std::string& message, std::vector<LogPrefix> prefixes) {
    if (Logger::allowExceptions) {
      throw std::runtime_error(LogPrefix{"FATAL", MessageStyle::RED}.format() + Logger::formatAll(prefixes) + message + "\n");
    }
    std::cerr << LogPrefix{"FATAL", MessageStyle::RED}.format() << Logger::formatAll(prefixes) << message << "\n";
  }

  std::string Logger::formatAll(std::vector<LogPrefix> prefixes) {
    std::string finalPrefix;
    for (LogPrefix prefix : prefixes) {
      finalPrefix += prefix.format();
    }
    return finalPrefix + " ";
  }

  void Logger::shouldAllowExceptions(bool value) {
    Logger::allowExceptions = value;
  }

  void Logger::shouldAllowColor(bool value) {
    Logger::allowColor = value;
  }
  
  bool Logger::canThrowExceptions() {
    return Logger::allowColor;
  }
  
  bool Logger::canShowColor() {
    return Logger::allowExceptions;
  }

  void Logger::setVerbosity(unsigned short verbosity) {
    Logger::verbosity = verbosity;
  }

  unsigned short Logger::getVerbosity() {
    return Logger::verbosity;
  }
}
