#include "PlcNonBlockingStringParser.h"


PlcNonBlockingStringParser::PlcNonBlockingStringParser()
{
	begin();
}
 void PlcNonBlockingStringParser::begin()
{
	 intNumber = 0;
	 isNegative = false;
	 isFraction = false;
	 fraction = 1.0;
	status = 0;
	size = 0;
	pos = 0;
	complete = false;
	data = "";
	az = 0;
}
 bool PlcNonBlockingStringParser::parseField(char c)
{
	 parseNumber(c);
	switch (status)
	{
	case 0:
		size = c << 8;
		status = 1;
		break;
	case 1:
		size = size | c;
		status = 2;
		break;
	case 2:
		appendChar(data, c);
		pos++;
		if (pos >= size)
		{
			status = 0;
			complete = true;
			return true;
		}
	}
	return false;
}
 bool PlcNonBlockingStringParser::parseField(PlcStream* stream)
{
	 if (complete) return true;
	if (stream == NULL) return 0;
	if (stream->availableIntern())
	{
		int r = stream->readIntern();
	
		if (r < 0)
		{
			PlcLogger::log(10, "NonBlockingStringParser/parseField/r<0",r);
			return false;
		}
		parseField(r);
		if (complete)
		{
			return true;
		}
	}
	return false;
}
  bool PlcNonBlockingStringParser::parseFixed(PlcStream* stream, int length)
 {
	  size = length;
	  if (complete) return true;
	  if (stream == NULL) return 0;
	  if (length == 0) return true;
	  if (stream->availableIntern())
	  {
		  int r = stream->readIntern();
	
		  if (r < 0)
		  {
			  PlcLogger::log(10, "NonBlockingStringParser/parseFixed/r<0",r);
			  return false;
		  }
		
		  az = r;
		  parseNumber(az);
		  appendChar(data, az);
		  pos++;
		  if (pos >= length)
		  {
			  complete = true;
			  return true;
		  }
	  }
	  return false;
 }
  bool PlcNonBlockingStringParser::parseByte(PlcStream* stream)
 {
	 if (stream == NULL) return 0;
	 if (stream->availableIntern())
	 {

	
		 PlcLogger::tempCounter++;
		 if (PlcLogger::tempCounter == 4)
		 {

			 int z = 0;
		 }

		 int r= stream->readIntern();
		
		 if (r < 0)
		 {
			// PlcLogger::log(10, "NonBlockingStringParser/parseByte/r<0", PlcLogger::tempCounter);
			 return false;
		 }
		 else
		 {
			 PlcLogger::log(10, "NonBlockingStringParser/parseByte/ byte Value", r);

		 }


		 az = r;
		 return true;
	 }
	 return false;
 }
  int PlcNonBlockingStringParser::parseNumber(char c)
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
   double PlcNonBlockingStringParser::getDouble()
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
  int PlcNonBlockingStringParser::getInt()
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
