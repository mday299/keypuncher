#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create a UDP socket
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Configure server address
    struct sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888); // Use the same port as the server
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    char buffer[1024];

    // Send data to the server
    const char *message = "Hello, server!";
    ssize_t bytesSent = sendto(clientSocket, message, strlen(message), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (bytesSent == -1) {
        std::cerr << "Error sending data" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Receive a response from the server
    ssize_t bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, nullptr, nullptr);
    if (bytesRead == -1) {
        std::cerr << "Error receiving data" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Print the received response
    buffer[bytesRead] = '\0';
    std::cout << "Received from server: " << buffer << std::endl;

    // Close the socket
    close(clientSocket);
    return 0;
}