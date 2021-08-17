# DebugLog

Serial based assertion and log library for Arduino

## Feature

- Printing variadic arguments in one line
- Release Mode `#define DEBUGLOG_DISABLE_LOG` can easily disable logging (`LOG_XXXX`, `ASSERT`)
- Log level control (`ERROR`, `WARN`, `INFO`, `DEBUG`, `TRACE`)
- Automatically or manually saving log to file
- Multiple file system support (`SD`, `SdFat`, `SPIFFS`, etc.)
- APIs can also be used in standard C++ apps

## Basic Usage

```C++
// Uncommenting DEBUGLOG_DISABLE_LOG disables ASSERT and all log (Release Mode)
// PRINT and PRINTLN are always valid even in Release Mode
// #define DEBUGLOG_DISABLE_LOG

// You can also set default log level by defining macro (default: INFO)
// #define DEBUGLOG_DEFAULT_LOG_LEVEL LogLevel::TRACE

#include <DebugLog.h>

void setup() {
    Serial.begin(115200);
    delay(2000);

    // PRINT and PRINTLN is not affected by log_level (always visible)
    PRINT("DebugLog", "can print variable args: ");
    PRINTLN(1, 2.2, "three", "=> like this");

    // You can change log_leval by following macro
    // LOG_SET_LEVEL(DebugLogLevel::TRACE);

    // The default log_leval is DebugLogLevel::INFO
    // 0: NONE, 1: ERROR, 2: WARN, 3: INFO, 4: DEBUG, 5: TRACE
    PRINTLN("current log level is", (int)LOG_GET_LEVEL());

    // The default log_leval is DebugLogLevel::INFO
    LOG_ERROR("this is error log");
    LOG_WARN("this is warn log");
    LOG_INFO("this is info log");
    LOG_DEBUG("this is debug log");  // won't be printed
    LOG_TRACE("this is trace log");  // won't be printed

    // Log array
    float arr[3] {1.1, 2.2, 3.3};
    PRINTLN("Array can be also printed like this", LOG_AS_ARR(arr, 3));

#if ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L  // Have libstdc++11
    // Log containers
    std::vector<int> vs {1, 2, 3};
    std::deque<float> ds {1.1, 2.2, 3.3};
    std::map<String, int> ms {{"one", 1}, {"two", 2}, {"three", 3}};
    PRINTLN("Containers can also be printed like", vs, ds, ms);
#endif

    delay(1000);

    // You can also use assert
    // If assertion failed, Serial endlessly prints message
    int x = 1;
    // ASSERT(x != 1);
    // You can also use assert with messages by ASSERTM macro
    ASSERTM(x != 1, "This always fails");
}
```

## Save Log to SD Card

```C++
// You can also set default file level by defining macro (default: ERROR)
// #define DEBUGLOG_DEFAULT_FILE_LEVEL LogLevel::WARN

// if you want to use standard SD library
#include <SD.h>
#define fs SD

// If you want to use SdFat
// #include <SdFat.h>
// SdFat fs;
// SdFatSdio fs;

// If you want use SPIFFS (ESP32) or other FileSystems
// #include <SPIFFS.h>
// #define fs SPIFFS

// after that, include DebugLog.h
#include <DebugLog.h>

void setup() {
    if (fs.begin()) {
        String filename = "test.txt";

        // Set file system to save every log automatically
        LOG_ATTACH_FS_AUTO(fs, filename, FILE_WRITE);

        // Set file system to save log manually
        // LOG_ATTACH_FS_MANUAL(fs, filename, FILE_WRITE);
    }

    // Apart from the log level to be displayed,
    // you can set the log level to be saved to a file (Default is LogLevel::ERROR)
    LOG_FILE_SET_LEVEL(DebugLogLevel::INFO);

    // If LOG_ATTACH_FS_AUTO is used, logs will be automatically saved to SD
    LOG_ERROR("error!");

    // If LOG_ATTACH_FS_MANUAL is used, you should manually save logs
    // however this is much faster than auto save (saving takes few milliseconds)
    LOG_FILE_FLUSH(); // manually save to SD card and continue logging
    // LOG_FILE_CLOSE(); // flush() and finish logging (ASSERT won't be saved to SD)
}
```

Please see `examples/log_to_file` , `examples/log_to_file_manual_save` for more details. And please note:

- One log function call can takes 3-20 ms if you log to file (depending on environment)
- If you disable auto save, you should call `LOG_FILE_FLUSH()` or `LOG_FILE_CLOSE()` to save logs

## APIs

| APIs                         | Serial | File     | Log Level    | Release Mode |
| ---------------------------- | ------ | -------- | ------------ | ------------ |
| `PRINT`, `PRINTLN`           | YES    | NO       | IGNORED      | ENABLED      |
| `PRINT_FILE`, `PRINTLN_FILE` | NO     | YES \*   | IGNORED      | ENABLED \*   |
| `ASSERT`, `ASSERTM`          | YES    | YES \*   | IGNORED      | DISABLED     |
| `LOG_XXXXX`                  | YES    | YES \*\* | CONTROLLABLE | DISABLED     |

`*` : Only after `LOG_FS_ATTACH_AUTO` or `LOG_FS_ATTACH_MANUAL` is called
`**` : Condition `*` is satisfied + only if `LOG_FILE_LEVEL` is matched

### Basic Logging Macros

`PRINT` and `PRINTLN` are available in both release and debug mode.

```C++
#define PRINT(...)
#define PRINTLN(...)
```

These logging APIs are enabled only in debug mode. Log level can control the visibility.

```C++
#define LOG_ERROR(...)
#define LOG_WARN(...)
#define LOG_INFO(...)
#define LOG_DEBUG(...)
#define LOG_TRACE(...)
```

Assertion suspends program if the condition is `true`.

```C++
#define ASSERT(b)
#define ASSERTM(b, msg)
```

## Logging to File

`PRINT_FILE` and `PRINTLN_FILE` are available in both release and debug mode.

```C++
#define PRINT_FILE(...)
#define PRINTLN_FILE(...)
```

If you use `LOG_ATTACH_FS_MANUAL`, these macros are used to flush files manually.

```C++
// Arduino Only (Manual operation)
#define LOG_FILE_FLUSH()
#define LOG_FILE_CLOSE()
```

### Log Option

```C++
#define LOG_AS_ARR(arr, size)
#define LOG_GET_LEVEL()
#define LOG_SET_LEVEL(level)
#define LOG_SET_OPTION(file, line, func)
#define LOG_SET_DELIMITER(delim)
#define LOG_SET_BASE_RESET(b)
// Arduino Only
#define LOG_ATTACH_SERIAL(serial)
#define LOG_FILE_IS_OPEN()
#define LOG_FILE_GET_LEVEL()
#define LOG_FILE_SET_LEVEL(lvl)
#define LOG_ATTACH_FS_AUTO(fs, path, mode)
#define LOG_ATTACH_FS_MANUAL(fs, path, mode)
```

## Option Definitions

### Log Level

```C++
enum class DebugLogLevel {
    NONE  = 0,
    ERROR = 1,
    WARN  = 2,
    INFO  = 3,
    DEBUG = 4,
    TRACE = 5
};
```

### Log Base

```C++
enum class DebugLogBase {
    DEC = 10,
    HEX = 16,
    OCT = 8,
    BIN = 2,  // only for Arduino
};
```

## Control Log Level Scope

You can control the scope of `DebugLog` by including following header files.

- `DebugLogEnable.h`
- `DebugLogDisable.h`
- `DebugLogRestoreState.h`

After including `DebugLogEnable.h` or `DebugLogDisable.h`, macros are enabled/disabled.
Finally you should include `DebugLogRestoreState.h` to restore the previous state.
Please see practical example `examples/control_scope` for details.

```C++
#define DEBUGLOG_DISABLE_LOG
#include <DebugLog.h>

// here is release mode (disable DebugLog)

#include <DebugLogEnable.h>

// here is debug mode (enable DebugLog)

#include <DebugLogRestoreState.h>

// here is release mode (restored)
```

## Used Inside of

- [ArduinoOSC](https://github.com/hideakitai/ArduinoOSC)
- [MsgPack](https://github.com/hideakitai/MsgPack)
- [ES920](https://github.com/hideakitai/ES920)
- [Sony9PinRemote](https://github.com/hideakitai/Sony9PinRemote)

## License

MIT
