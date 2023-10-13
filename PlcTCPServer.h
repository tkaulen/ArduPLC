#ifndef _PlcTCPServer_
#define _PlcTCPServer_
#include "PlcPlatform.h"
#ifdef _windows_
#include "PlcTCPStream.h"
class PlcTCPServer
{
	WSADATA wsaData;
	SOCKET serverSocket;
public:
	int startup(int port);
	int handleConnection(PlcTCPStream  *plcTCPStream);
};
#endif

#endif
