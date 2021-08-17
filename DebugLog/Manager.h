#pragma once
#ifndef DEBUGLOG_MANAGER_H
#define DEBUGLOG_MANAGER_H

#include "FileLogger.h"

namespace arx {
namespace debug {

    class Manager {
        LogLevel log_lvl {DEBUGLOG_DEFAULT_LOG_LEVEL};
        LogLevel curr_lvl {DEBUGLOG_DEFAULT_LOG_LEVEL};
        LogBase log_base {LogBase::DEC};
        string_t delim {" "};
        bool b_file {true};
        bool b_line {true};
        bool b_func {true};
        bool b_base_reset {true};

#ifdef ARDUINO
        Stream* stream {&Serial};
        FileLogger* logger {nullptr};
        LogLevel file_lvl {DEBUGLOG_DEFAULT_FILE_LEVEL};
        bool b_auto_save {false};
#endif

        // singleton
        Manager() {}
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;

    public:
        static Manager& get() {
            static Manager m;
            return m;
        }

        LogLevel log_level() const {
            return log_lvl;
        }

        void log_level(const LogLevel l) {
            log_lvl = l;
        }

        void option(const bool en_file, const bool en_line, const bool en_func) {
            b_file = en_file;
            b_line = en_line;
            b_func = en_func;
        }

        void delimiter(const string_t& del) {
            delim = del;
        }

        void base_reset(const bool b) {
            b_base_reset = b;
        }

#ifdef ARDUINO

        ~Manager() {
            if (logger) delete logger;
        }

        void attach(Stream& s) {
            stream = &s;
        }

        template <typename FsType, typename FileType, typename FileMode>
        void attach(FsType& s, const String& path, const FileMode& mode, const bool auto_save) {
            close();
            logger = new FsFileLogger<FsType, FileType>(s, path, mode);
            b_auto_save = auto_save;
        }

        void assertion(bool b, const char* file, int line, const char* func, const char* expr, const String& msg = "") {
            if (!b) {
                string_t str = string_t("[ASSERT] ") + file + string_t(" ") + line + string_t(" ") + func + string_t(" : ") + expr;
                if (msg.length()) str += string_t(" => ") + msg;
                stream->println(str);
                if (logger) {
                    logger->println(str);
                    logger->flush();
                }
                while (true)
                    ;
            }
        }

        void flush() {
            if (logger) logger->flush();
        }

        void close() {
            flush();
            if (logger) delete logger;
        }

        LogLevel file_level() const {
            return file_lvl;
        }

        void file_level(const LogLevel l) {
            file_lvl = l;
        }

#endif  // ARDUINO

        template <typename... Args>
        void log(LogLevel level, const char* file, int line, const char* func, Args&&... args) {
            bool b_ignore = (log_lvl == LogLevel::NONE);
#ifdef ARDUINO
            b_ignore &= (file_lvl == LogLevel::NONE);
#endif
            b_ignore |= (level == LogLevel::NONE);
            if (b_ignore) return;

            string_t header = generate_header(level, file, line, func);
            if ((int)level <= (int)log_lvl) {
                println(header, std::forward<Args>(args)...);
            }
#ifdef ARDUINO
            if (!logger) return;
            if ((int)level <= (int)file_lvl) {
                println_file(header, std::forward<Args>(args)...);
            }
#endif
        }

        // ===== print / println =====

        void print() {
            if (b_base_reset) log_base = LogBase::DEC;
        }

        template <typename Head, typename... Tail>
        void print(Head&& head, Tail&&... tail) {
#ifdef ARDUINO
            print_one(head, stream);
            if (sizeof...(tail) != 0)
                print_one(delim, stream);
#else
            print_one(head);
            if (sizeof...(tail) != 0)
                print_one(delim);
#endif
            print(std::forward<Tail>(tail)...);
        }

        void println() {
#ifdef ARDUINO
            print_one("\n", stream);
#else
            print_one("\n");
#endif
            if (b_base_reset) log_base = LogBase::DEC;
        }

        template <typename Head, typename... Tail>
        void println(Head&& head, Tail&&... tail) {
#ifdef ARDUINO
            print_one(head, stream);
            if (sizeof...(tail) != 0)
                print_one(delim, stream);
#else
            print_one(head);
            if (sizeof...(tail) != 0)
                print_one(delim);
#endif
            println(std::forward<Tail>(tail)...);
        }

#ifdef ARDUINO
        void print_file() {
            if (!logger) return;
            if (b_auto_save) logger->flush();
            print();
        }

        template <typename Head, typename... Tail>
        void print_file(Head&& head, Tail&&... tail) {
            if (!logger) return;
            print_one(head, logger);
            if (sizeof...(tail) != 0)
                print_one(delim, logger);
            print(std::forward<Tail>(tail)...);
        }

        void println_file() {
            if (!logger) return;
            print_one("\n", logger);
            print_file();
        }

        template <typename Head, typename... Tail>
        void println_file(Head&& head, Tail&&... tail) {
            print_one(head, logger);
            if (sizeof...(tail) != 0)
                print_one(delim, logger);
            println(std::forward<Tail>(tail)...);
        }
#endif

    private:
#ifdef ARDUINO

        // print without base
        template <typename Head, typename S>
        void print_one(Head&& head, S* s) { s->print(head); }

        // print with base
        template <typename S>
        void print_one(signed char head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_one(unsigned char head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_one(short head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_one(unsigned short head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_one(int head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_one(unsigned int head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_one(long head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_one(unsigned long head, S* s) { s->print(head, (int)log_base); }

        template <typename S>
        void print_one(LogBase& head, S*) {
            log_base = head;
        }

        template <typename S, typename T>
        void print_one(Array<T>& head, S* s) {
            print_array(head, s);
        }

#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L  // Have libstdc++11

        template <typename S, typename T>
        void print_one(vec_t<T>& head, S* s) {
            print_array(head, s);
        }

        template <typename S, typename T>
        void print_one(deq_t<T>& head, S* s) {
            print_array(head, s);
        }

        template <typename S, typename K, typename V>
        void print_one(map_t<K, V>& head, S* s) {
            print_map(head, s);
        }

#else  // Do not have libstdc++11

        template <typename S, typename T, size_t N>
        void print_one(vec_t<T, N>& head, S* s) {
            print_array(head, s);
        }

        template <typename S, typename T, size_t N>
        void print_one(deq_t<T, N>& head, S* s) {
            print_array(head, s);
        }

        template <typename S, typename K, typename V, size_t N>
        void print_one(map_t<K, V, N>& head, S* s) {
            print_map(head, s);
        }

#endif  // Do not have libstdc++11

        // print one helper
        template <typename S, typename T>
        void print_array(T& head, S* s) {
            print_one("[", s);
            for (size_t i = 0; i < head.size(); ++i) {
                print_one(head[i], s);
                if (i + 1 != head.size())
                    print_one(", ", s);
            }
            print_one("]", s);
        }

        template <typename S, typename T>
        void print_map(T& head, S* s) {
            print_one("{", s);
            const size_t size = head.size();
            size_t i = 0;
            for (const auto& kv : head) {
                print_one(kv.first, s);
                print_one(":", s);
                print_one(kv.second, s);
                if (++i != size)
                    print_one(", ", s);
            }
            print_one("}", s);
        }

#else

        template <typename Head>
        void print_one(Head&& head) {
            switch (log_base) {
                case LogBase::DEC: std::cout << std::dec; break;
                case LogBase::HEX: std::cout << std::hex; break;
                case LogBase::OCT: std::cout << std::oct; break;
            }
            std::cout << head;
        }

        template <typename T>
        void print_one(Array<T>& head) {
            print_array(head);
        }

        template <typename T>
        void print_one(vec_t<T>& head) {
            print_array(head);
        }

        template <typename T>
        void print_one(deq_t<T>& head) {
            print_array(head);
        }

        template <typename K, typename V>
        void print_one(map_t<K, V>& head) {
            print_map(head);
        }

        // print one helper
        template <typename T>
        void print_array(T& head) {
            print_one("[");
            for (size_t i = 0; i < head.size(); ++i) {
                print_one(head[i]);
                if (i + 1 != head.size())
                    print_one(", ");
            }
            print_one("]");
        }

        template <typename T>
        void print_map(T& head) {
            print_one("{");
            const size_t size = head.size();
            size_t i = 0;
            for (const auto& kv : head) {
                print_one(kv.first);
                print_one(":");
                print_one(kv.second);
                if (++i != size)
                    print_one(", ");
            }
            print_one("}");
        }

#endif

        // ===== other utilities =====

        string_t generate_header(const LogLevel lvl, const char* file, int line, const char* func) const {
            string_t header;
            switch (lvl) {
                case LogLevel::ERROR: header = "[ERROR] "; break;
                case LogLevel::WARN: header = "[WARN] "; break;
                case LogLevel::INFO: header = "[INFO] "; break;
                case LogLevel::DEBUG: header = "[DEBUG] "; break;
                case LogLevel::TRACE: header = "[TRACE] "; break;
                default: header = ""; break;
            }
#ifdef ARDUINO
            if (b_file) header += file + string_t(" ");
            if (b_line) header += string_t("L.") + line + string_t(" ");
            if (b_func) header += func + string_t(" ");
            header += string_t(": ");
#else
            if (b_file) {
                header += file;
                header += " ";
            };
            if (b_line) {
                header += "L.";
                header += std::to_string(line);
                header += " ";
            }
            if (b_func) {
                header += func;
                header += " ";
            };
            header += ": ";
#endif
            return header;
        }
    };

}  // namespace debug
}  // namespace arx

#endif  // DEBUGLOG_MANAGER_H
