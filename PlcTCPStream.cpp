#include "PlcTCPStream.h"
#ifdef _windows_
PlcTCPStream::PlcTCPStream() :PlcStream()
{
	clientSocket = NULL;

}
int PlcTCPStream::availableForWrite()
{
	if (clientSocket == NULL) return 0;
	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(clientSocket, &writeSet);
	timeval timeout;
	timeout.tv_sec = 0; // Timeout auf 0 setzen
	timeout.tv_usec = 0;
	int result = select(0, nullptr, &writeSet, nullptr, &timeout);
	if (result == SOCKET_ERROR)
	{
		//std::cout << "Error in select" << std::endl;
		return -1;
	}
	 if (result == 0)
	{
		// Socket ist nicht bereit zum Senden
		return 0;
	}
	 return 1;

}
size_t PlcTCPStream::write(uint8_t val)
{

	int v = val;
	//std::cout << v << std::endl;
	if (availableForWrite() == 0) return 0;
	buffer[0] = val;
	bufferSize = 1;
	//PlcLogger::logSpecial("Before send", 0);
	int bytesSent = send(clientSocket, buffer, bufferSize, 0);
	//PlcLogger::logSpecial("After send", bytesSent);
	if (bytesSent == SOCKET_ERROR)
	{
		// std::cout << "Error in send" << std::endl;
		return -1;
	}
	return bytesSent;
}
size_t PlcTCPStream::write(const uint8_t* bufferd, size_t size)
{
	
	//std::cout << v << std::endl;
	if (availableForWrite() == 0) return 0;

	//PlcLogger::logSpecial("Before send", 0);
	int bytesSent = send(clientSocket, (const char*)bufferd, size, 0);
	//PlcLogger::logSpecial("After send", bytesSent);
	if (bytesSent == SOCKET_ERROR)
	{
		// std::cout << "Error in send" << std::endl;
		return -1;
	}
	return bytesSent;
	
}
int PlcTCPStream::available()
{


	if (clientSocket == NULL) return 0;
	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(clientSocket, &readSet);
	timeval timeout;
	timeout.tv_sec = 0; // Timeout auf 0 setzen
	timeout.tv_usec = 0;
	int result = select(0, &readSet, nullptr, nullptr, &timeout);
	if (result == SOCKET_ERROR)
	{
		// std::cout << "Error in select" << std::endl;
		return 0;
	}

	/* bufferSize = 1;
	 int bytesRead = recv(clientSocket, buffer, bufferSize, 0);
	 if (bytesRead > 0)
	 {
		 storeAhead((unsigned char)buffer[0]);
		 return 1;
	 }*/

	return 1;
}
int PlcTCPStream::read()
{

	bufferSize = 1;
	int bytesRead = recv(clientSocket, buffer, bufferSize, 0);
	unsigned char bufferValue = (unsigned char)buffer[0];

	if (bytesRead > 0)
	{
		return (unsigned char)buffer[0];
	}
	return -1;
}
int PlcTCPStream::peek()
{
	return buffer[0];
}

#endif
