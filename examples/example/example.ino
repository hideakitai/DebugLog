// uncommend NDEBUG disables ASSERT and all debug serial (Release Mode)
//#define NDEBUG

#include <DebugLog.h>

void setup()
{
  Serial.begin(115200);
  delay(2000);

  // you can change target stream (default: Serial)
  // DebugLog::attach(Serial2);

  PRINT("this is for debug");
  PRINTLN(1, 2.2, "you can", "print variable args");
  PRINTLN("current log level is", (int)LOG_GET_LEVEL()); // 0: NONE, 1: ERRORS, 2: WARNINGS, 3: VERBOSE

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

  LOG_SET_LEVEL(DebugLogLevel::VERBOSE);
  PRINTLN("change log level to VERBOSE");
  LOG_SET_OPTION(false, false, false); // disable file, line, func
  PRINTLN("disable file, line, and func from output format");

  LOG_ERROR("this is error log");
  LOG_WARNING("this is warning log");
  LOG_VERBOSE("this is verbose log");

	LOG_SET_DELIMITER(" and ");
  PRINTLN("change delimtier from \" \" to \" and \"");
	LOG_VERBOSE(1, 2, 3, 4, 5);

  delay(5000);

  int x = 1;
  ASSERT(x != 1); // if assertion failed, Serial endlessly prints message
}

void loop()
{
  PRINTLN("if NDEBUG is commented out (assert is enabled), does not come here");
  delay(1000);
}
