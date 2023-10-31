#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
#endif

int main() {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize Winsock." << std::endl;
            return 1;
        }
    #endif

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        #ifdef _WIN32
            WSACleanup();
        #endif
        return 1;
    }

    // Connect to the server
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("169.254.137.119"); // IP address of the server
    serverAddr.sin_port = htons(12345); // Port number

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Connection failed." << std::endl;
        #ifdef _WIN32
            closesocket(clientSocket);
            WSACleanup();
        #endif
        return 1;
    }

    std::cout << "Connected to the server." << std::endl;

    // Send and receive data
    while (true) {
        std::string message;
        std::cout << "Enter a message (or 'exit' to quit): ";
        std::getline(std::cin, message);

        if (message == "exit")
            break;

        send(clientSocket, message.c_str(), message.size(), 0);

        char buffer[1024];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Connection closed or error occurred." << std::endl;
            break;
        }

        std::cout << "Received: " << std::string(buffer, bytesRead) << std::endl;
    }

    // Cleanup and close the client socket
    #ifdef _WIN32
        closesocket(clientSocket);
        WSACleanup();
    #else
        close(clientSocket);
    #endif

    return 0;
}
