#ifndef _PlcLogger_
#define _PlcLogger_

#include "PlcPlatform.h"

class PlcLogger
{

public:
	//static const int _logLevel=10;
	static const int _logLevel = 1;
	static  int tempCounter;
	static unsigned int timeCycle;



	static void logSpecial( String s, int value);
	static void log(int logLevel,String s);

	static void log(int logLevel, String s,int value);

	static void log(int logLevel, String s, const char *value);

	static void logTimed(int logLevel, String s, int value);
	static void writeOut(String s,int value);




};




#endif
