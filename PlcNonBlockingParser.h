#ifndef _PlcNonBlockingParser_
#define _PlcNonBlockingParser_
#include "PlcPlatform.h"
#include "PlcStream.h"
class PlcNonBlockingParser
{
public:
	long long intNumber;
	bool isNegative;
	bool isFraction;
	float fraction;
	char *symbol;

	char az;
	int length;
	int maxParseLength;
	
	boolean complete;
	PlcNonBlockingParser(int symbolMaxLength = 10);
	void beginParse();
	bool isSkip(char c);
	int parseNumber(char c);
	double getDouble();
	int getInt();
	bool acceptChar(char c);
	bool parseToken(char c);
	bool parseToken(PlcStream* stream);
	void endParse();
};
#endif
