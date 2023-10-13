#include "PlcStreamExtend.h"

#ifdef _windows_
PlcStreamExtend::PlcStreamExtend(PlcStream* stream)
{
    _stream = stream;
    fd = 0;
	plcStreamHelper = NULL;
   
}
#endif

#ifdef _arduino_
PlcStreamExtend::PlcStreamExtend(Stream* stream)
{
	plcStreamHelper = NULL;
    _stream = stream;
  
    fd = 0;
}

#endif



PlcStreamExtend::PlcStreamExtend() :PlcStream()
{
	plcStreamHelper = NULL;
	_stream = NULL;

}

size_t PlcStreamExtend::write(uint8_t val)
{
	if (plcStreamHelper != NULL) return plcStreamHelper->write(val);
	return _stream->write(val);
	
}

size_t PlcStreamExtend::write(const uint8_t* bufferd, size_t size)
{
	if (plcStreamHelper != NULL) return plcStreamHelper->write(bufferd, size);
	return _stream->write(bufferd, size);
}

int PlcStreamExtend::available()
{
	if (plcStreamHelper != NULL) return plcStreamHelper->available();
	return _stream->available();
	
}

int PlcStreamExtend::read()
{
	if (plcStreamHelper != NULL) return plcStreamHelper->read();
	
	int r = _stream->read();
	PlcLogger::log(10, "PlcStreamExtend/read", r);
	return r;
}

int PlcStreamExtend::peek()
{
	if (plcStreamHelper != NULL) return plcStreamHelper->peek();
	return _stream->peek();
}

int PlcStreamExtend::availableForWrite()
{
	if (plcStreamHelper != NULL) return plcStreamHelper->availableForWrite();
	return _stream->availableForWrite();

 }


