// uncommend NDEBUG disables ASSERT and all debug serial (Release Mode)
//#define NDEBUG

#include <Debug.h>

void setup()
{
  Serial.begin(115200);
  delay(2000);

  LOG_ERROR("this is error log");
  LOG_WARNING("this is warning log");
  LOG_VERBOSE("this is verbose log");

  LOG_SET_LEVEL(Debug::LogLevel::WARNING);
  Serial.println("change log level to WARNING");

  LOG_ERROR("this is error log");
  LOG_WARNING("this is warning log");
  LOG_VERBOSE("this is verbose log");

  LOG_SET_LEVEL(Debug::LogLevel::ERROR);
  Serial.println("change log level to ERROR");

  LOG_ERROR("this is error log");
  LOG_WARNING("this is warning log");
  LOG_VERBOSE("this is verbose log");

  LOG_SET_LEVEL(Debug::LogLevel::NONE);
  Serial.println("change log level to NONE");

  LOG_ERROR("this is error log");
  LOG_WARNING("this is warning log");
  LOG_VERBOSE("this is verbose log");

  delay(5000);

  int x = 1;
  ASSERT(x != 1); // if assertion failed, Serial endlessly prints message
}

void loop()
{
  Serial.println("if NDEBUG is commented out (assert is enabled), does not come here");
  delay(1000);
}
