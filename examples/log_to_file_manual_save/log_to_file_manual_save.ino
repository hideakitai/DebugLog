// Uncommenting DEBUGLOG_DISABLE_MACRO disables ASSERT and all log (Release Mode)
// PRINT and PRINTLN are always valid even in Release Mode
// #define DEBUGLOG_DISABLE_MACRO

// You can also set default log level by defining macro
// #define DEBUGLOG_DEFAULT_LOGLEVEL LogLevel::WARN

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
// #include <LittleFS.h>
// TODO:
// #include <FatFs.h>
// TODO:

#include <DebugLog.h>

void shorten(String& s) {
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] == ':')
            s.setCharAt(i, '_');
    }
}

void print_all_files() {
    File root = fs.open("/");
    while (true) {
        File entry = root.openNextFile();
        if (!entry) {
            LOG_INFO("All files read");
            break;
        }
        LOG_INFO("=====", entry.name(), "=====");
        if (entry.isDirectory()) {
            ;  // ignore directory
        } else {
            while (entry.available()) {
                PRINT((char)entry.read());
            }
            PRINTLN();
        }
        entry.close();
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    if (fs.begin()) {
        PRINTLN("FileSystem initialization success");

        print_all_files();

        String filename = "/" + String(__TIME__) + ".txt";
        shorten(filename);

        // 3rd arg: true: auto save every logging, false: manually save
        // 4th arg: true: only log to SD, false: also print via Serial
        LOG_ATTACH_FS(fs, filename, true, false);
    } else {
        PRINTLN("sd initialization failed!");
        while (true)
            ;
    }

    // Apart from the log level to be displayed,
    // you can set the log level to be saved to a file (Default is LogLevel::INFO)
    LOG_SET_SAVE_LEVEL(DebugLogLevel::ERROR);

    // ===== Following process is the same as basic.ino =====

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

    // ===== End of the process same as the basic.ino =====

    LOG_FS_FLUSH();  // save to SD card and continue logging
    // LOG_FS_CLOSE(); // flush() and finish logging (ASSERT won't be saved to SD)

    delay(1000);

    // assertions are automatically saved if DebugLog is not closed
    // if DebugLog is closed, assertions won't be saved to SD
    int x = 1;
    // ASSERT(x != 1);
    // You can also use assert with messages by ASSERTM macro
    ASSERTM(x != 1, "This always fails");
}

void loop() {
    PRINTLN("if DEBUGLOG_DISABLE_MACRO is commented out (assert is enabled), does not come here");
    delay(1000);
}
