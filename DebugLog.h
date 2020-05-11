#pragma once
#ifndef ARX_DEBUGLOG_H
#define ARX_DEBUGLOG_H

#ifdef ARDUINO
    #include <Arduino.h>
    #include <stdarg.h>
#else
    #include <iostream>
#endif


namespace arx {
namespace debug {

    namespace detail
    {
        template <class T> struct remove_reference      { using type = T; };
        template <class T> struct remove_reference<T&>  { using type = T; };
        template <class T> struct remove_reference<T&&> { using type = T; };
        template <class T> constexpr T&& forward(typename remove_reference<T>::type& t) noexcept { return static_cast<T&&>(t); }
        template <class T> constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept { return static_cast<T&&>(t); }
    }

    inline void print() { Serial.print(" "); }

    template<typename Head>
    inline void print(Head&& head)
    {
#ifdef ARDUINO
        Serial.print(head);
#else
        std::cout << head;
#endif
        print();
    }

    template<typename Head, typename... Tail>
    inline void print(Head&& head, Tail&&... tail)
    {
#ifdef ARDUINO
        Serial.print(head);
        Serial.print(" ");
        print(detail::forward<Tail>(tail)...);
#else
        std::cout << head << " ";
        print(std::forward<Tail>(tail)...);
#endif
    }

    inline void println()
    {
#ifdef ARDUINO
        Serial.println();
#else
        std::cout << std::endl;
#endif
    }

    template<typename Head>
    inline void println(Head&& head)
    {
#ifdef ARDUINO
        Serial.print(head);
#else
        std::cout << head;
#endif
        println();
    }

    template<typename Head, typename... Tail>
    inline void println(Head&& head, Tail&&... tail)
    {
#ifdef ARDUINO
        Serial.print(head);
        Serial.print(" ");
        println(detail::forward<Tail>(tail)...);
#else
        std::cout << head << " ";
        println(std::forward<Tail>(tail)...);
#endif
    }

#ifdef ARDUINO
    inline void assert(bool b, const char* file, int line, const char* func, const char* expr)
    {
        while (!b)
        {
            println("[ASSERT] ", file, ":", line, ":", func, "() : ", expr);
            delay(1000);
        }
    }
#endif

    enum class LogLevel {NONE, ERROR, WARNING, VERBOSE};
    LogLevel log_level = LogLevel::VERBOSE;

    void log(LogLevel level, const char* file, int line, const char* func, const char* expr)
    {
        if ((log_level == LogLevel::NONE) || (level == LogLevel::NONE)) return;
        if ((int)level <= (int)log_level)
        {
            String lvl_str;
            if      (level == LogLevel::ERROR)   lvl_str = "ERROR";
            else if (level == LogLevel::WARNING) lvl_str = "WARNING";
            else if (level == LogLevel::VERBOSE) lvl_str = "VERBOSE";
            println("[", lvl_str, "] ", file, ":", line, ":", func, "() : ", expr);
        }
    }

} // namespace debug
} // namespace ard

#ifdef NDEBUG

#define PRINT(...) ((void)0)
#define PRINTLN(...) ((void)0)
#define LOG_ERROR(s,...) ((void)0)
#define LOG_WARNING(s,...) ((void)0)
#define LOG_VERBOSE(s,...) ((void)0)
#define ASSERT(b) ((void)0)

#else // NDEBUG

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define PRINT(...) arx::debug::print(__VA_ARGS__)
#define PRINTLN(...) arx::debug::println(__VA_ARGS__)
#define LOG_ERROR(s,...) arx::debug::log(arx::debug::LogLevel::ERROR, __FILENAME__, __LINE__, __func__, s)
#define LOG_WARNING(s,...) arx::debug::log(arx::debug::LogLevel::WARNING, __FILENAME__, __LINE__, __func__, s)
#define LOG_VERBOSE(s,...) arx::debug::log(arx::debug::LogLevel::VERBOSE, __FILENAME__, __LINE__, __func__, s)
#ifdef ARDUINO
    #define ASSERT(b) arx::debug::assert((b), __FILENAME__, __LINE__, __func__, #b)
#else
    #include <cassert>
    #define ASSERT(b) assert(b)
#endif

#endif // #ifdef NDEBUG


#define LOG_SET_LEVEL(lvl) arx::debug::log_level = lvl
using DebugLogLevel = arx::debug::LogLevel;


#endif // ARX_DEBUGLOG_H
