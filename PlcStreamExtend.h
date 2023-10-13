#ifndef _PlcStreamExtend_
#define _PlcStreamExtend_
#include "PlcPlatform.h"
#include "PlcLogger.h"


#include "PlcStream.h"
#include "PlcStreamHelper.h"

#ifdef _arduino_
#include "Arduino.h"
#include "Client.h"
#endif


class PlcStreamHelper;

class PlcStreamExtend : public PlcStream
{
public:
	PlcStreamHelper* plcStreamHelper;
	int fd;

	

#ifdef _windows_
	PlcStream* _stream;
	PlcStreamExtend(PlcStream *stream);

#endif
#ifdef _arduino_
	Stream* _stream;
	PlcStreamExtend(Stream* stream);

#endif



	PlcStreamExtend();

	virtual size_t write(uint8_t val) override;
	virtual size_t write(const uint8_t* bufferd, size_t size) override;
	virtual int available() override;
	virtual int read() override;
	virtual int peek() override;
	virtual int availableForWrite() override;
	





};




#endif