#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Logger {
public:
  enum class LogLevel { INFO, WARNING, ERROR };

  virtual ~Logger() = default;
  virtual void log(LogLevel level, const std::string &message) = 0;
};

class ConsoleLogger : public Logger {
public:
  void log(LogLevel level, const std::string &message) override {
    std::cout << getLevelPrefix(level) << message << std::endl;
  }

private:
  std::string getLevelPrefix(LogLevel level) {
    switch (level) {
    case LogLevel::INFO:
      return "[INFO] ";
    case LogLevel::WARNING:
      return "[WARNING] ";
    case LogLevel::ERROR:
      return "[ERROR] ";
    default:
      return "[UNKNOWN] ";
    }
  }
};

class FileLogger : public Logger {
public:
  explicit FileLogger(const std::string &filename) {
    file.open(filename, std::ios::app);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open log file: " + filename);
    }
  }

  ~FileLogger() override {
    if (file.is_open()) {
      file.close();
    }
  }

  void log(LogLevel level, const std::string &message) override {
    if (file.is_open()) {
      file << getLevelPrefix(level) << message << std::endl;
    }
  }

private:
  std::ofstream file;

  std::string getLevelPrefix(LogLevel level) {
    switch (level) {
    case LogLevel::INFO:
      return "[INFO] ";
    case LogLevel::WARNING:
      return "[WARNING] ";
    case LogLevel::ERROR:
      return "[ERROR] ";
    default:
      return "[UNKNOWN] ";
    }
  }
};

class MultiLogger : public Logger {
public:
  void addLogger(std::shared_ptr<Logger> logger) { loggers.push_back(logger); }

  void log(LogLevel level, const std::string &message) override {
    for (const auto &logger : loggers) {
      logger->log(level, message);
    }
  }

private:
  std::vector<std::shared_ptr<Logger>> loggers;
};

#endif // LOGGER_H
