#pragma once
#ifndef DEBUGLOG_MANAGER_H
#define DEBUGLOG_MANAGER_H

#include "FileLogger.h"

namespace arx {
namespace debug {

    class Manager {
        LogLevel log_level {DEBUGLOG_DEFAULT_LOGLEVEL};
        LogLevel curr_level {DEBUGLOG_DEFAULT_LOGLEVEL};
        LogBase log_base {LogBase::DEC};
        string_t delim {" "};
        bool b_file {true};
        bool b_line {true};
        bool b_func {true};
        bool b_base_reset {true};

#ifdef ARDUINO
        Stream* stream {&Serial};
        FileLogger* logger {nullptr};
        LogLevel save_level {DEBUGLOG_DEFAULT_LOGLEVEL};
        bool b_auto_save {false};
        bool b_only_fs {false};
#endif

        // singleton
        Manager() {}
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;

    public:
        ~Manager() {
#ifdef ARDUINO
            if (logger) delete logger;
#endif
        }

        static Manager& get() {
            static Manager m;
            return m;
        }

#ifdef ARDUINO
        void attach(Stream& s) { stream = &s; }

        void flush() {
            if (logger) logger->flush();
        }

        void close() {
            flush();
            if (logger) delete logger;
        }

#ifdef FILE_WRITE
        template <typename FsType, typename FileMode>
        void attach(FsType& s, const String& path, const FileMode& mode, const bool auto_save, const bool only_fs) {
            close();
            logger = new FsFileLogger<FsType, File>(s, path, mode);
            b_auto_save = auto_save;
            b_only_fs = only_fs;
        }
#endif

#endif  // ARDUINO

        void print() {
#ifdef ARDUINO
            if (logger && b_auto_save) logger->flush();
#endif
            if (b_base_reset) log_base = LogBase::DEC;
        }

        template <typename Head, typename... Tail>
        void print(Head&& head, Tail&&... tail) {
            print_impl(head, (sizeof...(tail) == 0));
#ifdef ARDUINO
            print(detail::forward<Tail>(tail)...);
#else
            print(std::forward<Tail>(tail)...);
#endif
        }

        void println() {
#ifdef ARDUINO
            if (!b_only_fs) {
                stream->println();
            }
            if (logger) {
                logger->println();
                if (b_auto_save) logger->flush();
            }
#else
            std::cout << std::endl;
#endif
            if (b_base_reset) log_base = LogBase::DEC;
        }

        template <typename Head, typename... Tail>
        void println(Head&& head, Tail&&... tail) {
            print_impl(head, (sizeof...(tail) == 0));
#ifdef ARDUINO
            println(detail::forward<Tail>(tail)...);
#else
            println(std::forward<Tail>(tail)...);
#endif
        }

#ifdef ARDUINO
        void assertion(bool b, const char* file, int line, const char* func, const char* expr, const String& msg = "") {
            while (!b) {
                string_t str = string_t("[ASSERT] ") + file + string_t(" ") + line + string_t(" ") + func + string_t(" : ") + expr;
                if (msg.length()) str += string_t(" => ") + msg;
                if (!b_only_fs)
                    stream->println(str);
                if (logger) {
                    logger->println(str);
                    logger->flush();
                }
                delay(5000);
            }
        }
#endif

        void logLevel(const LogLevel l) { log_level = l; }
        LogLevel logLevel() const { return log_level; }

#ifdef ARDUINO
        void saveLevel(const LogLevel l) { save_level = l; }
        LogLevel saveLevel() const { return save_level; }
#endif

        template <typename... Args>
        void log(LogLevel level, const char* file, int line, const char* func, Args&&... args) {
            curr_level = level;
            if ((log_level == LogLevel::NONE) || (curr_level == LogLevel::NONE)) return;
            if ((int)curr_level <= (int)log_level) {
                string_t header = get_log_level_header();
#ifdef ARDUINO
                if (b_file) header += file + string_t(" ");
                if (b_line) header += string_t("L.") + line + string_t(" ");
                if (b_func) header += func + string_t(" ");
                header += string_t(": ");
                print(header);
                println(detail::forward<Args>(args)...);
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
                print(header);
                println(std::forward<Args>(args)...);
#endif
            }
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

    private:
        template <typename Head>
        void print_impl(Head&& head, const bool b_last_idx) {
#ifdef ARDUINO
            if (!b_only_fs) {
                print_exec(head, stream);
                if (!b_last_idx)
                    stream->print(delim);
            }
            if (logger && ((int)curr_level <= (int)save_level)) {
                print_exec(head, logger);
                if (!b_last_idx)
                    logger->print(delim);
            }
#else
            print_exec(head);
            if (!b_last_idx)
                std::cout << delim;
#endif
        }

        template <typename T>
        void print_impl(Array<T>& head, const bool b_last_idx) {
            print_array(head, b_last_idx);
        }

#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L  // Have libstdc++11

        template <typename T>
        void print_impl(vec_t<T>& head, const bool b_last_idx) {
            print_array(head, b_last_idx);
        }

        template <typename T>
        void print_impl(deq_t<T>& head, const bool b_last_idx) {
            print_array(head, b_last_idx);
        }

        template <typename K, typename V>
        void print_impl(map_t<K, V>& head, const bool b_last_idx) {
            print_impl("{", false);
            for (const auto& kv : head) {
                print_impl(kv.first, false);
                print_impl(":", false);
                print_impl(kv.second, false);
                print_impl(",", false);
            }
            print_impl("}", b_last_idx);
        }

#else  // Do not have libstdc++11

        template <typename T, size_t N>
        void print_impl(vec_t<T, N>& head, const bool b_last_idx) {
            print_array(head, b_last_idx);
        }

        template <typename T, size_t N>
        void print_impl(deq_t<T, N>& head, const bool b_last_idx) {
            print_array(head, b_last_idx);
        }

        template <typename K, typename V, size_t N>
        void print_impl(map_t<K, V, N>& head, const bool b_last_idx) {
            print_impl("{", false);
            for (const auto& kv : head) {
                print_impl(kv.first, false);
                print_impl(":", false);
                print_impl(kv.second, false);
                print_impl(",", false);
            }
            print_impl("}", b_last_idx);
        }

#endif  // Do not have libstdc++11

        void print_impl(LogBase& head, const bool) {
            log_base = head;
        }

        template <typename T>
        void print_array(T& arr, const bool b_last_idx) {
            print_impl("[", false);
            for (size_t i = 0; i < arr.size(); ++i) {
                print_impl(arr[i], false);
            }
            print_impl("]", b_last_idx);
        }

#ifdef ARDUINO
        template <typename Head, typename S>
        void print_exec(Head&& head, S* s) { s->print(head); }
        template <typename S>
        void print_exec(signed char head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_exec(unsigned char head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_exec(short head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_exec(unsigned short head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_exec(int head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_exec(unsigned int head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_exec(long head, S* s) { s->print(head, (int)log_base); }
        template <typename S>
        void print_exec(unsigned long head, S* s) { s->print(head, (int)log_base); }
#else
        template <typename Head>
        void print_exec(Head&& head) {
            switch (log_base) {
                case LogBase::DEC: std::cout << std::dec; break;
                case LogBase::HEX: std::cout << std::hex; break;
                case LogBase::OCT: std::cout << std::oct; break;
            }
            std::cout << head;
        }
#endif

        string_t get_log_level_header() const {
            string_t lvl_str;
            switch (curr_level) {
                case LogLevel::ERROR:
                    lvl_str = "[ERROR] ";
                    break;
                case LogLevel::WARN:
                    lvl_str = "[WARN] ";
                    break;
                case LogLevel::INFO:
                    lvl_str = "[INFO] ";
                    break;
                case LogLevel::DEBUG:
                    lvl_str = "[DEBUG] ";
                    break;
                case LogLevel::TRACE:
                    lvl_str = "[TRACE] ";
                    break;
                default:
                    lvl_str = "";
                    break;
            }
            return lvl_str;
        }
    };

}  // namespace debug
}  // namespace arx

#endif  // DEBUGLOG_MANAGER_H
