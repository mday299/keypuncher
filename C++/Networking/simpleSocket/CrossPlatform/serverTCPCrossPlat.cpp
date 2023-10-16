#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }
#endif

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Bind the socket
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345); // Port number
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
#ifdef _WIN32
        closesocket(serverSocket);
        WSACleanup();
#else   
        close(serverSocket);
#endif
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
#ifdef _WIN32
        closesocket(serverSocket);
        WSACleanup();
#else
        close(serverSocket);
#endif
        return 1;
    }

    std::cout << "Server listening on port 12345..." << std::endl;

    // Accept incoming connections
    SOCKET clientSocket;
    sockaddr_in clientAddress;
    int clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed." << std::endl;
        closesocket(serverSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Communication with the client
    char buffer[1024];
    int bytesRead;
    while (true) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == 0)
            break;
        if (bytesRead == SOCKET_ERROR) {
            std::cerr << "Receive error." << std::endl;
            closesocket(clientSocket);
            closesocket(serverSocket);
#ifdef _WIN32
            WSACleanup();
#endif
            return 1;
        }
        // Handle received data
        buffer[bytesRead] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        // Echo the received data back to the client
        send(clientSocket, buffer, bytesRead, 0);
    }

    // Close sockets and clean up
    closesocket(clientSocket);
    closesocket(serverSocket);
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
