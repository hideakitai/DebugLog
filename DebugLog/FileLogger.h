#pragma once
#ifndef DEBUGLOG_FILE_LOGGER_H
#define DEBUGLOG_FILE_LOGGER_H

#include "Types.h"

namespace arx {
namespace debug {

#ifdef ARDUINO

    struct FileLogger {
        virtual ~FileLogger() {}
        virtual void flush() = 0;

        virtual size_t print(const __FlashStringHelper*) = 0;
        virtual size_t print(const String&) = 0;
        virtual size_t print(const char[]) = 0;
        virtual size_t print(char) = 0;
        virtual size_t print(unsigned char, int = DEC) = 0;
        virtual size_t print(int, int = DEC) = 0;
        virtual size_t print(unsigned int, int = DEC) = 0;
        virtual size_t print(long, int = DEC) = 0;
        virtual size_t print(unsigned long, int = DEC) = 0;
        virtual size_t print(double, int = 2) = 0;
        virtual size_t print(const Printable&) = 0;

        virtual size_t println(const __FlashStringHelper*) = 0;
        virtual size_t println(const String&) = 0;
        virtual size_t println(const char[]) = 0;
        virtual size_t println(char) = 0;
        virtual size_t println(unsigned char, int = DEC) = 0;
        virtual size_t println(int, int = DEC) = 0;
        virtual size_t println(unsigned int, int = DEC) = 0;
        virtual size_t println(long, int = DEC) = 0;
        virtual size_t println(unsigned long, int = DEC) = 0;
        virtual size_t println(double, int = 2) = 0;
        virtual size_t println(const Printable&) = 0;
        virtual size_t println(void) = 0;
    };

    template <typename FsType, typename FileType>
    class FsFileLogger : public FileLogger {
        FsType* fs;
        String path;
        FileType file;

    public:
        FsFileLogger(FsType& fsclass, const String& path)
        : fs(&fsclass), path(path) {
#ifdef O_WRITE
            file = fs->open(path.c_str(), O_WRITE | O_CREAT);
#elif defined(FILE_WRITE)
            file = fs->open(path.c_str(), FILE_WRITE);
#endif
        }

        virtual ~FsFileLogger() {
            if (file) file.close();
        }
        virtual void flush() override { file.flush(); }

        virtual size_t print(const __FlashStringHelper* x) override { return file.print(x); }
        virtual size_t print(const String& x) override { return file.print(x); }
        virtual size_t print(const char x[]) override { return file.print(x); }
        virtual size_t print(char x) override { return file.print(x); }
        virtual size_t print(unsigned char x, int b = DEC) override { return file.print(x, b); }
        virtual size_t print(int x, int b = DEC) override { return file.print(x, b); }
        virtual size_t print(unsigned int x, int b = DEC) override { return file.print(x, b); }
        virtual size_t print(long x, int b = DEC) override { return file.print(x, b); }
        virtual size_t print(unsigned long x, int b = DEC) override { return file.print(x, b); }
        virtual size_t print(double x, int b = 2) override { return file.print(x, b); }
        virtual size_t print(const Printable& x) override { return file.print(x); }

        virtual size_t println(const __FlashStringHelper* x) override { return file.println(x); }
        virtual size_t println(const String& x) override { return file.println(x); }
        virtual size_t println(const char x[]) override { return file.println(x); }
        virtual size_t println(char x) override { return file.println(x); }
        virtual size_t println(unsigned char x, int b = DEC) override { return file.println(x, b); }
        virtual size_t println(int x, int b = DEC) override { return file.println(x, b); }
        virtual size_t println(unsigned int x, int b = DEC) override { return file.println(x, b); }
        virtual size_t println(long x, int b = DEC) override { return file.println(x, b); }
        virtual size_t println(unsigned long x, int b = DEC) override { return file.println(x, b); }
        virtual size_t println(double x, int b = 2) override { return file.println(x, b); }
        virtual size_t println(const Printable& x) override { return file.println(x); }
        virtual size_t println(void) override { return file.println(); }
    };

#endif  // ARDUINO

}  // namespace debug
}  // namespace arx

#endif  // DEBUGLOG_FILE_LOGGER_H
