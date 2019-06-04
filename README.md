# Debug
Serial based assertion and log library for Arduino

## Usage

```C++
// uncommend NDEBUG disables ASSERT and all debug serial (Release Mode)
//#define NDEBUG

#include <Debug.h>

void setup()
{
    PRINT("this is for debug", 1, 2.2, "you can", "print variable args");

    LOG_SET_LEVEL(Debug::LogLevel::ERROR); // only ERROR log is printed
    LOG_SET_LEVEL(Debug::LogLevel::WARNING); // ERROR and WARNING is printed
    LOG_SET_LEVEL(Debug::LogLevel::VERBOSE); // all log is printed

    LOG_ERROR("this is error log");
    LOG_WARNING("this is warning log");
    LOG_VERBOSE("this is verbose log");

    int x = 1;
    ASSERT(x != 1); // if assertion failed, Serial endlessly prints message
}
```

## License

MIT

