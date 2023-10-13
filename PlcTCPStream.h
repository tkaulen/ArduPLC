#ifndef _PlcTCPStream_
#define _PlcTCPStream_
#include "PlcStream.h"
#include "PlcPlatform.h"
#ifdef _windows_
//#include <winsock.h>
//#pragma comment(lib, "ws2_32.lib")
//#include <ws2tcpip.h>
class PlcTCPStream : public PlcStream
{
public:
	
	SOCKET clientSocket;
	sockaddr_in clientAddress;
	 char buffer[20];
	int bufferSize;
	PlcTCPStream();
	int availableForWrite();
	// Geerbt über PlcStream
	virtual size_t write(uint8_t val) override;
	virtual size_t write(const uint8_t* bufferd, size_t size) override;
	virtual int available() override;
	virtual int read() override;
	virtual int peek() override;
};
#endif
#endif
