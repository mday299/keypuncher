#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const char* SERVER_IP = "127.0.0.1";
const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {
    // Create socket
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return -1;
    }

    // Server address
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << strerror(errno) << std::endl;
        close(clientSocket);
        return -1;
    }

    // Send data to the server
    const char* message = "Hello, Server!";
    if (sendto(clientSocket, message, strlen(message), 0, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error sending data: " << strerror(errno) << std::endl;
        close(clientSocket);
        return -1;
    }

    // Receive response from the server
    char buffer[BUFFER_SIZE];
    int recvBytes = recvfrom(clientSocket, buffer, BUFFER_SIZE - 1, 0, nullptr, nullptr);
    if (recvBytes == -1) {
        std::cerr << "Error receiving response: " << strerror(errno) << std::endl;
        close(clientSocket);
        return -1;
    }

    buffer[recvBytes] = '\0';
    std::cout << "Received from server: " << buffer << std::endl;

    // Close the socket
    close(clientSocket);

    return 0;
}
