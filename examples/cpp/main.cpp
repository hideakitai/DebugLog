//#define NDEBUG

#include "../../DebugLog.h"

int main()
{
	PRINT("this is for debug");
	PRINTLN(1, 2.2, "you can", "print variable args");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	LOG_SET_LEVEL(DebugLogLevel::WARNINGS);
	PRINTLN("change log level to WARNINGS");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	LOG_SET_LEVEL(DebugLogLevel::ERRORS);
	PRINTLN("change log level to ERRORS");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	LOG_SET_LEVEL(DebugLogLevel::NONE);
	PRINTLN("change log level to NONE");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	LOG_SET_LEVEL(DebugLogLevel::VERBOSE);
	PRINTLN("change log level to VERBOSE");
	LOG_SET_OPTION(false, false, false);
	PRINTLN("disable file, line, and func from output format");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	LOG_SET_DELIMITER(" and ");
	PRINTLN("change delimtier from \" \" to \" and \"");
	LOG_VERBOSE(1, 2, 3, 4, 5);

	int x = 1;
	ASSERT(x != 1); // in c++ app, standard assert is used
}
