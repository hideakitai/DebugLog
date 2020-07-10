//#define NDEBUG

#include "../../DebugLog.h"

int main()
{
	PRINT("this is for debug");
	PRINTLN(1, 2.2, "you can", "print variable args");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	DebugLog::logLevel(DebugLogLevel::WARNINGS);
	PRINTLN("change log level to WARNINGS");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	DebugLog::logLevel(DebugLogLevel::ERRORS);
	PRINTLN("change log level to ERRORS");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	DebugLog::logLevel(DebugLogLevel::NONE);
	PRINTLN("change log level to NONE");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	int x = 1;
	ASSERT(x != 1); // in c++ app, standard assert is used
}