#include "PlcNonBlockingParser.h"


PlcNonBlockingParser::PlcNonBlockingParser(int symbolMaxLength)
{
	symbol = (char*)malloc(symbolMaxLength);
	beginParse();
	maxParseLength = symbolMaxLength;;
}
void PlcNonBlockingParser::beginParse()
{
	intNumber = 0;
	isNegative = false;
	isFraction = false;
	fraction = 1.0;
	*symbol = 0;
	az = 0;
	length = 0;
	complete = false;
}
bool PlcNonBlockingParser::isSkip(char c)
{
	return true;
}
int PlcNonBlockingParser::parseNumber(char c)
{
	if (isFraction)
	{
		fraction *= 0.1;
	}
	if (c == '-')
	{
		isNegative = true;
		return 1;
	}
	if (c >= '0' && c <= '9')
	{
		intNumber = intNumber * 10 + c - '0';
		return 1;
	}
	if (c == '.' || c == ',')
	{
		isFraction = true;
		return 1;
	}
	return 0;
}
double PlcNonBlockingParser::getDouble()
{
	if (isNegative)
	{
		return -(intNumber * fraction);
	}
	else
	{
		return intNumber * fraction;
	}
}
int PlcNonBlockingParser::getInt()
{
	if (isFraction)
	{
		return (int)getDouble();
	}
	if (isNegative)
	{
		return -(intNumber * fraction);
	}
	else
	{
		return intNumber * fraction;
	}
}
bool PlcNonBlockingParser::acceptChar(char c)
{
	if (c == '-') return true;
	if (c == '.') return true;
	if (c == ',') return true;
	if (c >= 48 && c <= 57) return true;
	if (c >= 65 && c <= 122) return true;
	return false;
}
bool PlcNonBlockingParser::parseToken(char c)
{
	if (complete)
	{
		return true;
	}
	if (c == ' ')
	{
		az = c;
		return false;
	}
	if (!acceptChar(c))
	{
		complete = true;
		az = c;
		return true;;
	}
	if (length > maxParseLength)
	{
		complete = true;
		az = c;
		return true;
	}
	az = c;
	symbol[length] = az;
	//symbol += c;
	
	//symbol.append(1, c);
	parseNumber(c);
	length++;
	symbol[length] = 0;
	return false;
}
bool PlcNonBlockingParser::parseToken(PlcStream* stream)
{
	if (complete) return true;
	if (stream == NULL) return 0;
	if (stream->availableIntern())
	{
		int r = stream->readIntern();

		if (r < 0)
		{
			return false;
		}
		parseToken(r);
		if (complete) return true;
	}
	return complete;
}
void PlcNonBlockingParser::endParse()
{
	double d = getDouble();
	int i = getInt();
	int u = 0;
}