#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {
    // Create socket
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return -1;
    }

    // Bind to a specific port
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding to port " << PORT << ": " << strerror(errno) << std::endl;
        close(serverSocket);
        return -1;
    }

    // Receive data from clients
    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    while (true) {
        int recvBytes = recvfrom(serverSocket, buffer, BUFFER_SIZE - 1, 0, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
        if (recvBytes == -1) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            break;
        }

        buffer[recvBytes] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;

        // You can add your processing logic here

        // Send a response (optional)
        const char* response = "Message received";
        if (sendto(serverSocket, response, strlen(response), 0, reinterpret_cast<struct sockaddr*>(&clientAddr), clientAddrLen) == -1) {
            std::cerr << "Error sending response: " << strerror(errno) << std::endl;
            break;
        }
    }

    // Close the socket
    close(serverSocket);

    return 0;
}
