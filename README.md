# DebugLog

Serial based assertion and log library for Arduino

## Feature

- Printing variadic arguments in one line
- Release mode : `#define DEBUGLOG_RELEASE_MODE` can disables debug info (`LOG_XXXX`)
- Log level control (`ERROR`, `WARN`, `INFO`, `DEBUG`, `TRACE`)
- Automatically or manually saving log to file
- Multiple file system support (`SdFat`, `SPIFFS`, etc.)
- APIs can be used in standard C++ apps

## Basic Usage

```C++
// Uncommenting DEBUGLOG_RELEASE_MODE disables ASSERT and all log (Release Mode)
// PRINT and PRINTLN are always valid even in Release Mode
// #define DEBUGLOG_RELEASE_MODE

// You can also set default log level by defining macro
// #define DEBUGLOG_DEFAULT_LOGLEVEL LogLevel::WARN

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
    // PRINTLN("current log level is", (int)LOG_GET_LEVEL());

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
    ASSERT(x != 1);
}
```

## Save Log to SD Card

```C++
// if you want to use standard SD library
#include <SD.h>
#define fs SD

// if you want to use SdFat
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
      LOG_ATTACH_SD(fs, filename, false, true);
      // 3rd arg => true: auto save every logging, false: manually save
      // 4th arg => true: only log to SD, false: also print via Serial
    }

    // Apart from the log level to be displayed,
    // you can set the log level to be saved to a file (Default is LogLevel::INFO)
    LOG_SET_SAVE_LEVEL(DebugLogLevel::ERROR);

    // if 3rd arg is true, logs will be automatically saved to SD
    LOG_ERROR("error!");

    // if 3rd arg is false, you should manually save logs
    // however this is much faster than auto save (saving takes few milliseconds)
    LOG_FS_FLUSH(); // manually save to SD card and continue logging
    // LOG_FS_CLOSE(); // flush() and finish logging (ASSERT won't be saved to SD)
}
```

Please see `examples/log_to_file` , `examples/log_to_file_manual_save` for more details. And please note:

- one log function call can takes 3-20 ms if you log to file (depending on environment)
- if you disable auto save, you should call `LOG_FS_FLUSH()` or `LOG_FS_CLOSE()` to save logs

## Control Log Level Scope

You can control the scope of `DebugLog` by including following header files.

- `DebugLogEnable.h`
- `DebugLogDisable.h`
- `DebugLogRestoreState.h`

After including `DebugLogEnable.h` or `DebugLogDisable.h`, macros are enabled/disabled.
Finally you should include `DebugLogRestoreState.h` to restore the previous state.
Please see practical example `examples/control_scope` for details.

```C++
#define DEBUGLOG_RELEASE_MODE
#include <DebugLog.h>

// here is release mode (disable DebugLog)

#include <DebugLogEnable.h>

// here is debug mode (enable DebugLog)

#include <DebugLogRestoreState.h>

// here is release mode (restored)
```

## APIs

### Basic Logging Macros

`PRINT` and `PRINTLN` are available in both release and debug mode.

```C++
#define PRINT(...)
#define PRINTLN(...)
```

These logging APIs are enabled only in debug mode.

```C++
#define LOG_ERROR(...)
#define LOG_WARN(...)
#define LOG_INFO(...)
#define LOG_DEBUG(...)
#define LOG_TRACE(...)
#define ASSERT(b)
```

### Logging to File

```C++
// Arduino Only (Manual operation)
#define LOG_FS_FLUSH()
#define LOG_FS_CLOSE()
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
#define LOG_GET_SAVE_LEVEL() DebugLog::Manager::get().saveLevel()
#define LOG_SET_SAVE_LEVEL(l) DebugLog::Manager::get().saveLevel(l)
#define LOG_ATTACH_SERIAL(s)
#define LOG_ATTACH_FS(fs, path, auto_save, only_sd)
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

## Used Inside of

- [ArduinoOSC](https://github.com/hideakitai/ArduinoOSC)
- [MsgPack](https://github.com/hideakitai/MsgPack)
- [ES920](https://github.com/hideakitai/ES920)
- [Sony9PinRemote](https://github.com/hideakitai/Sony9PinRemote)

## License

MIT
