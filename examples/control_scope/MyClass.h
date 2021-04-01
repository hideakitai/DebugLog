#pragma once
#ifndef MYCLASS_H
#define MYCLASS_H

#include <Arduino.h>
#include <DebugLog.h>

// you can control the scope depending on your own flag
#ifdef MYCLASS_ENABLE_DEBUGLOG
#include <DebugLogEnable.h>
#else
#include <DebugLogDisable.h>
#endif  // MYCLASS_ENABLE_DEBUGLOG

class MyClass {
public:
    MyClass() {
        LOG_VERBOSE("MyClass created");
    }
    ~MyClass() {
        LOG_VERBOSE("MyClass destroyed");
    }
    void hello() {
        LOG_VERBOSE("hello", "world");
    }
};

// finally you need to restore default DebugLog status (release or debug)
#include "DebugLogRestoreState.h"

#endif  // MYCLASS_H
