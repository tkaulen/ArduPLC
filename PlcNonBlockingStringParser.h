#ifndef _PlcNobBlockingStringParser_
#define _PlcNobBlockingStringParser_
#include "PlcPlatform.h"
#include "PlcStream.h"
#include "PlcLogger.h"
class PlcNonBlockingStringParser
{
public:
	long long intNumber;
	bool isNegative;
	bool isFraction;
	float fraction;
	int status;
	int16_t size;
	int16_t pos;
	String data;
	boolean complete;
	 char az;
	PlcNonBlockingStringParser();
	void begin();
	bool parseField(char c);
	bool parseField(PlcStream* stream);
	bool parseFixed(PlcStream* stream, int length);
	bool parseByte(PlcStream* stream);
	int parseNumber(char c);
	double getDouble();
	int getInt();
};
#endif
