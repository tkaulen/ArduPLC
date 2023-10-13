#pragma once
#ifdef _WIN32
#define _windows_
#else
#define _arduino_
#endif


#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif


#ifdef _arduino_
#include <Arduino.h>
#define appendChar(dst,src) dst.concat(src);

#endif


#define Int32 int32_t

#ifdef _windows_
#include <string>
#include <chrono>
#include <iostream>
#include <Windows.h>
#include <conio.h>
//#include "WString.h"
#define String string
#define appendChar(dst,src) dst.append(1, src);
using namespace std;
using namespace chrono;

int millis();
#endif
