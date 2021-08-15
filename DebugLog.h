#pragma once
#ifndef ARX_DEBUGLOG_H
#define ARX_DEBUGLOG_H

#ifdef ARDUINO
#include <Arduino.h>
#include <stdarg.h>
#else
#include <iostream>
#include <string>
#include <string.h>
#endif

#include "DebugLog/util/ArxContainer/ArxContainer.h"
#include "DebugLog/Types.h"
#include "DebugLog/Manager.h"

namespace DebugLog = arx::debug;
using DebugLogLevel = arx::debug::LogLevel;
using DebugLogBase = arx::debug::LogBase;

// PRINT and PRINTLN are always enabled regardless of debug mode or release mode
#define PRINT(...) DebugLog::Manager::get().print(__VA_ARGS__)
#define PRINTLN(...) DebugLog::Manager::get().println(__VA_ARGS__)
#define LOG_AS_ARR(arr, sz) DebugLog::to_arr(arr, sz)
#define LOG_GET_LEVEL() DebugLog::Manager::get().logLevel()
#define LOG_SET_LEVEL(l) DebugLog::Manager::get().logLevel(l)
#define LOG_SET_OPTION(file, line, func) DebugLog::Manager::get().option(file, line, func)
#define LOG_SET_DELIMITER(d) DebugLog::Manager::get().delimiter(d)
#define LOG_SET_BASE_RESET(b) DebugLog::Manager::get().base_reset(b)
#ifdef ARDUINO
#define LOG_GET_SAVE_LEVEL() DebugLog::Manager::get().saveLevel()
#define LOG_SET_SAVE_LEVEL(l) DebugLog::Manager::get().saveLevel(l)
#define LOG_ATTACH_SERIAL(s) DebugLog::Manager::get().attach(s)
#define LOG_ATTACH_FS(s, p, b, ...) DebugLog::Manager::get().attach(s, p, b, __VA_ARGS__)
#endif  // ARDUINO

#include "DebugLogRestoreState.h"

#endif  // ARX_DEBUGLOG_H
