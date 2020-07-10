#pragma once
#ifndef ARX_DEBUGLOG_H
#define ARX_DEBUGLOG_H

#ifdef ARDUINO
    #include <Arduino.h>
    #include <stdarg.h>
#else
    #include <iostream>
    #include <string>
    #include <string.h>
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

// serial loggers
#ifdef ARDUINO
    using string_t = String;
    Stream* stream {&Serial};
    inline void attach(Stream& s) { stream = &s; }
#else
    using string_t = std::string;
#endif

// sd loggers
#ifdef ARDUINO

    struct Logger
    {
        virtual ~Logger() {}
        virtual void print(const String&) = 0;
        virtual void println(const String&) = 0;
        virtual void println() = 0;
        virtual void flush() = 0;
    };

    template <typename SdType, typename FileType>
    class SdLogger : public Logger
    {
        SdType* sd;
        String path;
        FileType file;

    public:

        SdLogger(SdType& sdclass, const String& path)
        : sd(&sdclass)
        , path(path)
        {
#ifdef FILE_WRITE
            file = sd->open(path.c_str(), O_WRITE | O_CREAT);
#endif
        }

        virtual ~SdLogger() { if (file) file.close(); }

        virtual void print(const String& str) override { file.print(str); }
        virtual void println(const String& str) override { file.println(str); }
        virtual void println() override { file.println(); }
        virtual void flush() override { file.flush(); }
    };

    Logger* logger {nullptr};
    bool b_auto_save {false};
    bool b_only_sd {false};

    inline void flush()
    {
        if (logger) logger->flush();
    }

    inline void close()
    {
        flush();
        if (logger) delete logger;
    }

#ifdef FILE_WRITE
    template <typename SdType>
    inline void attach(SdType& s, const String& path, bool auto_save, bool only_sd = false)
    {
        close();
        logger = new SdLogger<SdType, File>(s, path);
        b_only_sd = only_sd;
    }
#endif

#endif // ARDUINO

    inline void print()
    {
#ifdef ARDUINO
        if (logger && b_auto_save) logger->flush();
#endif
    }

    template<typename Head, typename... Tail>
    inline void print(Head&& head, Tail&&... tail)
    {
#ifdef ARDUINO
        if (!b_only_sd)
        {
            stream->print(head);
            stream->print(" ");
        }
        if (logger)
        {
            logger->print(head);
            logger->print(" ");
        }
        print(detail::forward<Tail>(tail)...);
#else
        std::cout << head << " ";
        print(std::forward<Tail>(tail)...);
#endif
    }

    inline void println()
    {
#ifdef ARDUINO
        if (!b_only_sd)
        {
            stream->println();
        }
        if (logger)
        {
            logger->println();
            if (b_auto_save) logger->flush();
        }
#else
        std::cout << std::endl;
#endif
    }

    template<typename Head, typename... Tail>
    inline void println(Head&& head, Tail&&... tail)
    {
#ifdef ARDUINO
        if (!b_only_sd)
        {
            stream->print(head);
            stream->print(" ");
        }
        if (logger)
        {
            logger->print(head);
            logger->print(" ");
        }
        println(detail::forward<Tail>(tail)...);
#else
        std::cout << head << " ";
        println(std::forward<Tail>(tail)...);
#endif
    }

#ifdef ARDUINO
    inline void assertion(bool b, const char* file, int line, const char* func, const char* expr)
    {
        while (!b)
        {
            println("[ ASSERT ]", file, ":", line, ":", func, "() :", expr);
            if (logger) logger->flush();
            delay(1000);
        }
    }
#endif

    enum class LogLevel {NONE, ERRORS, WARNINGS, VERBOSE};
    LogLevel log_level = LogLevel::VERBOSE;

    inline void logLevel(const LogLevel l) { log_level = l; }
    inline LogLevel logLevel() { return log_level; }

    template <typename... Args>
    inline void log(LogLevel level, const char* file, int line, const char* func, Args&&... args)
    {
        if ((log_level == LogLevel::NONE) || (level == LogLevel::NONE)) return;
        if ((int)level <= (int)log_level)
        {
            string_t lvl_str;
            switch (level)
            {
                case LogLevel::ERRORS:   lvl_str = "ERROR";   break;
                case LogLevel::WARNINGS: lvl_str = "WARNING"; break;
                case LogLevel::VERBOSE:  lvl_str = "VERBOSE"; break;
                default:                 lvl_str = "";        break;
            }
#ifdef ARDUINO
            println("[", lvl_str, "]", file, ":", line, ":", func, ":", detail::forward<Args>(args)...);
#else
            println("[", lvl_str, "]", file, ":", line, ":", func, ":", std::forward<Args>(args)...);
#endif
        }
    }

} // namespace debug
} // namespace ard

#ifdef NDEBUG

#define PRINT(...) ((void)0)
#define PRINTLN(...) ((void)0)
#define LOG_ERROR(...) ((void)0)
#define LOG_WARNING(...) ((void)0)
#define LOG_VERBOSE(...) ((void)0)
#define ASSERT(b) ((void)0)

#else // NDEBUG

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define PRINT(...) arx::debug::print(__VA_ARGS__)
#define PRINTLN(...) arx::debug::println(__VA_ARGS__)
#define LOG_ERROR(...) arx::debug::log(arx::debug::LogLevel::ERRORS, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define LOG_WARNING(...) arx::debug::log(arx::debug::LogLevel::WARNINGS, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define LOG_VERBOSE(...) arx::debug::log(arx::debug::LogLevel::VERBOSE, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#ifdef ARDUINO
    #define ASSERT(b) arx::debug::assertion((b), __FILENAME__, __LINE__, __func__, #b)
#else
    #include <cassert>
    #define ASSERT(b) assert(b)
#endif

#endif // #ifdef NDEBUG


namespace DebugLog = arx::debug;
using DebugLogLevel = arx::debug::LogLevel;


#endif // ARX_DEBUGLOG_H
