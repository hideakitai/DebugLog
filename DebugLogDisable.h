#undef LOG_ERROR
#undef LOG_WARN
#undef LOG_INFO
#undef LOG_DEBUG
#undef LOG_TRACE
#ifdef ARDUINO
#undef LOG_SD_FLUSH
#undef LOG_SD_CLOSE
#endif
#undef ASSERT

#define LOG_ERROR(...) ((void)0)
#define LOG_WARN(...) ((void)0)
#define LOG_INFO(...) ((void)0)
#define LOG_DEBUG(...) ((void)0)
#define LOG_TRACE(...) ((void)0)
#ifdef ARDUINO
#define LOG_SD_FLUSH() ((void)0)
#define LOG_SD_CLOSE() ((void)0)
#endif
#define ASSERT(b) ((void)0)
