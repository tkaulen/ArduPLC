#ifndef _PlcStreamHelper_
#define _PlcStreamHelper_
#include "PlcPlatform.h"
#include "PlcStreamExtend.h"


class PlcStreamExtend;

class PlcStreamHelper
{

public:
	PlcStreamExtend* plcStream;

	size_t write(uint8_t val);
	virtual size_t write(const uint8_t* bufferd, size_t size);
	virtual int available() ;
	virtual int read() ;
	virtual int peek() ;
	virtual int availableForWrite() ;

};











#endif
