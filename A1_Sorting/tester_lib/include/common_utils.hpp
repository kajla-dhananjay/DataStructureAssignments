#pragma once
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#ifdef DEBUG
#define DEBUG_LOG_LABEL(label, msg)                                  \
  do {                                                                 \
    auto now = std::chrono::system_clock::now();                       \
    auto now_time = std::chrono::system_clock::to_time_t(now);         \
    struct tm tm_buf;                                                  \
    localtime_r(&now_time, &tm_buf);                                   \
    char time_str[100];                                                \
    std::strftime(time_str, sizeof(time_str), "%d %b %Y %H:%M:%S", &tm_buf); \
    std::cerr << "[" << label << "][" << time_str << "] " << msg << "\n"; \
  } while (0)

#define DEBUG_LOG(msg) DEBUG_LOG_LABEL("DEBUG", msg)
#else
#define DEBUG_LOG_LABEL(label, msg)
#define DEBUG_LOG(msg)
#endif


template <typename T>
struct TypeName {
  static const char* name() {
    if constexpr (std::is_same_v<T, int>) {
      return "int";
    } else if constexpr (std::is_same_v<T, std::string>) {
      return "string";
    } else {
      return "unknown";
    }
  }
};