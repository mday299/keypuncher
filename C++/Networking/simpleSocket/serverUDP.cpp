#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int main() {
  // Create a socket
  int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverSocket == -1) {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  // Bind the socket
  sockaddr_in serverAddress;
  std::memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8888);  // Port number
  serverAddress.sin_addr.s_addr =
      INADDR_ANY;  // Bind to any available interface

  int reuse = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse,
                 sizeof(reuse)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
  }

  if (bind(serverSocket, (struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) == -1) {
    std::cerr << "Error binding socket" << std::endl;
    close(serverSocket);
    return 2;
  }

  char buffer[1024];
  sockaddr_in clientAddress;
  socklen_t clientAddrLen = sizeof(clientAddress);

  // Receive data from the client
  ssize_t bytesRead =
      recvfrom(serverSocket, buffer, sizeof(buffer), 0,
               (struct sockaddr *)&clientAddress, &clientAddrLen);
  if (bytesRead == -1) {
    std::cerr << "Error receiving data" << std::endl;
    close(serverSocket);
    return 1;
  }

  // Print received data
  buffer[bytesRead] = '\0';
  std::cout << "Received from client: " << buffer << std::endl;

  // Respond to the client
  const char *response = "Hello, client!";
  ssize_t bytesSent = sendto(serverSocket, response, strlen(response), 0,
                             (struct sockaddr *)&clientAddress, clientAddrLen);
  if (bytesSent == -1) {
    std::cerr << "Error sending data" << std::endl;
    close(serverSocket);
    return 1;
  }

  // Close socket
  close(serverSocket);

  return 0;
}