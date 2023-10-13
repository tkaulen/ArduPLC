#ifndef _PlcPrintStream_
#define _PlcPrintStream_
#include "PlcStream.h"
#include "PlcPlatform.h"
#ifdef _windows_


class PlcPrintStream : public PlcStream
{
public:
	PlcPrintStream();
	HANDLE hInput;
	DWORD prevMode;
	int key;
	boolean hasNext;
	boolean getNext();
	int availableForWrite();
	void begin(int baudrate)
	{

	}
	// Geerbt über PlcStream
	virtual size_t write(uint8_t val) override;
	virtual int available() override;
	virtual int read() override;
	virtual int peek() override;
};
#endif



#ifdef _arduino_


#define PlcPrintStream Serial

#define PlcPrintStream1 Serial1




#endif

#endif
