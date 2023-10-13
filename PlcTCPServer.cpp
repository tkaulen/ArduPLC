#include "PlcTCPServer.h"
#ifdef _windows_
#define WM_SOCKET (WM_USER + 1)
 int PlcTCPServer::startup(int port)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "Failed to initialize winsock" << std::endl;
		return -1;
	}
	// Erstelle einen Socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Failed to create socket" << std::endl;
		return -1;
	}
	// Definiere die Serveradresse und Port
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port); // Telnet-Port: 23
	// Binde den Socket an die Serveradresse
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		std::cout << "Failed to bind socket" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	// Horche auf eingehende Verbindungen
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Failed to listen on socket" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	std::cout << "Waiting for incoming connections..." << std::endl;
	// Setze den Server-Socket auf den nicht blockierenden Modus
	u_long nonBlocking = 1;
	if (ioctlsocket(serverSocket, FIONBIO, &nonBlocking) != 0)
	{
		std::cout << "Failed to set server socket to non-blocking mode" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return -1;
	}
	return 0;
}
 int PlcTCPServer::handleConnection(PlcTCPStream* plcTCPStream)
{
	 // Setze die Menge der Sockets, die auf Ereignisse überwacht werden sollen
	 fd_set readSockets;
	 FD_ZERO(&readSockets);
	 FD_SET(serverSocket, &readSockets);
	 timeval timeout;
	 timeout.tv_sec = 0; // Timeout auf 0 setzen
	 timeout.tv_usec = 0;
	 // Finde den größten Socket-Deskriptor
	 int maxSocket = serverSocket;
		 // Warte auf Ereignisse auf den Sockets
		 fd_set temp = readSockets;
		 int socketCount = select(0, &temp, nullptr, nullptr, &timeout);
		 if (socketCount == SOCKET_ERROR)
		 {
			 std::cout << "select() failed" << std::endl;
			 return 0;
		 }
		 // Überprüfe eingehende Verbindungen
		 if (FD_ISSET(serverSocket, &temp))
		 {
			 // Akzeptiere die Verbindung
			 SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
			 if (clientSocket != INVALID_SOCKET)
			 {
				 std::cout << "Client connected" << std::endl;
				 // Füge den Client-Socket zur Menge der überwachten Sockets hinzu
				 FD_SET(clientSocket, &readSockets);
				 // Aktualisiere den größten Socket-Deskriptor, falls erforderlich
				 if (clientSocket > maxSocket)
				 {
					 maxSocket = clientSocket;
				 }
				 plcTCPStream->clientSocket = clientSocket;
				 return 1;
			 }
		 }
	return 0;
}

#endif
