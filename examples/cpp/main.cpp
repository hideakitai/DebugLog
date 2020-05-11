//#define NDEBUG

#include "../../DebugLog.h"

int main(int argc, char *argv[]) 
{
	PRINT("this is for debug");
	PRINTLN(1, 2.2, "you can", "print variable args");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	LOG_SET_LEVEL(DebugLogLevel::WARNING);
	PRINTLN("change log level to WARNING");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	LOG_SET_LEVEL(DebugLogLevel::ERROR);
	PRINTLN("change log level to ERROR");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	LOG_SET_LEVEL(DebugLogLevel::NONE);
	PRINTLN("change log level to NONE");

	LOG_ERROR("this is error log");
	LOG_WARNING("this is warning log");
	LOG_VERBOSE("this is verbose log");

	int x = 1;
	ASSERT(x != 1); // in c++ app, standard assert is used
}