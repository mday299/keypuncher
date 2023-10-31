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

    while (true) {
        // Accept incoming connections
        struct sockaddr_in clientAddr;
        #ifdef _WIN32
            int clientAddrSize = sizeof(clientAddr);
        #else
            socklen_t clientAddrSize = sizeof(clientAddr);
        #endif
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            break;
        }

        std::cout << "Client connected." << std::endl;

        // Main loop to receive and echo data
        char buffer[1024];
        int bytesRead;
        while (true) {
            bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                std::cerr << "Connection closed or error occurred." << std::endl;
                break;
            }

            // Echo received data back to the client
            send(clientSocket, buffer, bytesRead, 0);
        }

        // Close the client socket
        #ifdef _WIN32
            closesocket(clientSocket);
        #else
            close(clientSocket);
        #endif

        std::cout << "Client disconnected." << std::endl;
    }

    // Cleanup and close the server socket
    #ifdef _WIN32
        closesocket(serverSocket);
        WSACleanup();
    #else
        close(serverSocket);
    #endif

    return 0;
}
