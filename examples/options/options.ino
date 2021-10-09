// Uncommenting DEBUGLOG_DISABLE_LOG disables ASSERT and all log (Release Mode)
// PRINT and PRINTLN are always valid even in Release Mode
// #define DEBUGLOG_DISABLE_LOG

// You can also set default log level by defining macro (default: INFO)
// #define DEBUGLOG_DEFAULT_LOG_LEVEL DebugLogLevel::LVL_TRACE

#include <DebugLog.h>

void setup() {
    Serial.begin(115200);
    delay(2000);

    // You can change target stream (default: Serial)
    // LOG_ATTACH_SERIAL(Serial2);

    // You can change auto reset for base setting (default: true)
    PRINTLN("You can print variable args with bases",
        DebugLogBase::BIN, 85,
        DebugLogBase::OCT, 85,
        DebugLogBase::DEC, 85,
        DebugLogBase::HEX, 85);

    // You can change auto reset for base setting (default: true)
    // LOG_SET_BASE_RESET(false);
    PRINTLN("This would be hex", DebugLogBase::HEX, 255);
    PRINTLN("If you set LOG_SET_BASE_RESET(false), this would be hex", 255);

    // You can change the delimiter
    LOG_SET_DELIMITER(" and ");
    PRINTLN("Changed delimtier from \" \" to \" and \"");
    LOG_INFO(1, 2, 3, 4, 5);

    // You can change some log format options
    LOG_SET_OPTION(false, false, false);  // disable file, line, func
    PRINTLN("Disabled file, line, and func from log output format");
    LOG_INFO("You can't see file, line, and func here");
}

void loop() {
}
