// uncommend NDEBUG disables ASSERT and all debug serial (Release Mode)
// #define NDEBUG

// if you want to use standard SD library
#include <SD.h>

// if you want to use SdFat
// #include <SdFat.h>
// SdFat SD;
// SdFatSdio SD;

#include <DebugLog.h>

void shorten(String& s) {
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] == ':')
            s.setCharAt(i, '_');
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    if (SD.begin()) {
        PRINTLN("sd initialization success");

        String filename = "/" + String(__TIME__) + ".txt";
        shorten(filename);

        // 3rd arg: true: auto save every logging, false: manually save
        // 4th arg: true: only log to SD, false: also print via Serial
        LOG_ATTACH_SD(SD, filename, false, true);
    } else
        PRINTLN("sd initialization failed!");

    PRINT("this is for debug");
    PRINTLN(1, 2.2, "you can", "print variable args");
    PRINTLN("current log level is", (int)LOG_GET_LEVEL());  // 0: NONE, 1: ERRORS, 2: WARNINGS, 3: VERBOSE

    LOG_ERROR("this is error log");
    LOG_WARNING("this is warning log");
    LOG_VERBOSE("this is verbose log");

    LOG_SET_LEVEL(DebugLogLevel::WARNINGS);
    PRINTLN("change log level to WARNINGS");
    PRINTLN("current log level is", (int)LOG_GET_LEVEL());

    LOG_ERROR("this is error log");
    LOG_WARNING("this is warning log");
    LOG_VERBOSE("this is verbose log");

    LOG_SET_LEVEL(DebugLogLevel::ERRORS);
    PRINTLN("change log level to ERRORS");
    PRINTLN("current log level is", (int)LOG_GET_LEVEL());

    LOG_ERROR("this is error log");
    LOG_WARNING("this is warning log");
    LOG_VERBOSE("this is verbose log");

    LOG_SET_LEVEL(DebugLogLevel::NONE);
    PRINTLN("change log level to NONE");
    PRINTLN("current log level is", (int)LOG_GET_LEVEL());

    LOG_ERROR("this is error log");
    LOG_WARNING("this is warning log");
    LOG_VERBOSE("this is verbose log");

    LOG_SD_FLUSH();  // save to SD card and continue logging
    // LOG_SD_CLOSE(); // flush() and finish logging (ASSERT won't be saved to SD)

    delay(5000);

    // assertions are automatically saved if DebugLog is not closed
    // if DebugLog is closed, assertions won't be saved to SD
    int x = 1;
    ASSERT(x != 1);  // if assertion failed, Serial endlessly prints message
}

void loop() {
    PRINTLN("if NDEBUG is commented out (assert is enabled), does not come here");
    delay(1000);
}
