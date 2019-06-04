#pragma once
#ifndef EMBEDDEDUTILS_DEBUG_H
#define EMBEDDEDUTILS_DEBUG_H

#include <Arduino.h>
#include <stdarg.h>

#if defined(TEENSYDUINO) || defined(__AVR__) || defined(__MBED__) || defined (ESP_PLATFORM) || defined (ESP8266)

namespace ard {
namespace Debug {

    void Assert(bool b, const char* file, int line, const char* func, const char* expr)
    {
        while (!b)
        {
#ifndef __AVR__
            Serial.printf("[ASSERT] %s:%d:%s() : %s\n", file, line, func, expr);
#else
            Serial.print("[ASSERT] ");
            Serial.print(file); Serial.print(":");
            Serial.print(line); Serial.print(":");
            Serial.print(func); Serial.print("() : ");
            Serial.println(expr);
#endif
        }
    }

    enum class LogLevel {NONE, ERROR, WARNING, VERBOSE};
    LogLevel log_level = LogLevel::VERBOSE;

    void Log(LogLevel level, const char* file, int line, const char* func, const char* expr)
    {
        if ((log_level == LogLevel::NONE) || (level == LogLevel::NONE)) return;
        if ((int)level <= (int)log_level)
        {
            String lvl_str;
            if      (level == LogLevel::ERROR)   lvl_str = "ERROR";
            else if (level == LogLevel::WARNING) lvl_str = "WARNING";
            else if (level == LogLevel::VERBOSE) lvl_str = "VERBOSE";
#ifndef __AVR__
            Serial.printf("[%s] %s:%d:%s() : %s\n", lvl_str.c_str(), file, line, func, expr);
#else
            Serial.print("["); Serial.print(lvl_str); Serial.print("] ");
            Serial.print(file); Serial.print(":");
            Serial.print(line); Serial.print(":");
            Serial.print(func); Serial.print("() : ");
            Serial.println(expr);
#endif
        }
    }

    void print() { Serial.println(); }

    template<typename Head, typename... Tail>
    void print(Head&& head, Tail&&... tail)
    {
        Serial.print(head);
        Serial.print(" ");
        print(std::forward<Tail>(tail)...);
    }

} // namespace Debug
} // namespace ard

#ifdef NDEBUG

#define ASSERT(b) ((void)0)
#define LOG_ERROR(s,...) ((void)0)
#define LOG_WARNING(s,...) ((void)0)
#define LOG_VERBOSE(s,...) ((void)0)
#define PRINT(s,...) ((void)0)

#else // NDEBUG

#define ASSERT(b) ard::Debug::Assert((b), __FILE__, __LINE__, __func__, #b)
#define LOG_ERROR(s,...) ard::Debug::Log(ard::Debug::LogLevel::ERROR, __FILE__, __LINE__, __func__, s)
#define LOG_WARNING(s,...) ard::Debug::Log(ard::Debug::LogLevel::WARNING, __FILE__, __LINE__, __func__, s)
#define LOG_VERBOSE(s,...) ard::Debug::Log(ard::Debug::LogLevel::VERBOSE, __FILE__, __LINE__, __func__, s)
#define PRINT(s,...) ard::Debug::print(s, __VA_ARGS__)

#endif // #ifdef NDEBUG

#define LOG_SET_LEVEL(lvl) ard::Debug::log_level = lvl
using DebugLogLevel = ard::Debug::LogLevel;

#endif // #if defined(TEENSYDUINO) || defined(__AVR__) || defined(__MBED__) || defined (ESP_PLATFORM) || defined (ESP8266)

#endif // EMBEDDEDUTILS_DEBUG_H
