#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#define MAX 500
#define port 5200

int main() {
  int clientSocket, serverSocket, receiveMsgSize;
  clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    std::cout << "Creation of client socket failed" << std::endl;
    return 0;
  }
  struct sockaddr_in serverAddr, clientAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serverAddr.sin_port = htons(port);
  if (connect(clientSocket, (struct sockaddr *)&serverAddr,
              sizeof(serverAddr)) < 0) {
    std::cout << "Connection Error..." << std::endl;
    return 0;
  } else {
    std::cout << "\t\tConnection Established..." << std::endl;
  }

  while (true) {
    std::string s;
    char input[MAX];
    std::cout << "Client : ";
    getline(std::cin, s);
    int n = s.size();
    for (int i = 0; i < n; i++) {
      input[i] = s[i];
    }
    input[n] = '\0';
    send(clientSocket, input, strlen(input) + 1, 0);
    char receiveMessage[MAX];
    int rMsgSize = recv(clientSocket, receiveMessage, MAX, 0);
    if (rMsgSize < 0) {
      std::cout << "Packet recieve failed." << std::endl;
      return 0;
    } else if (rMsgSize == 0) {
      std::cout << "Server is off." << std::endl;
      return 0;
    }

    if (receiveMessage[0] == 'b' && receiveMessage[1] == 'y' &&
        receiveMessage[2] == 'e') {
      std::cout << "\nConnection ended... take care bye bye... ";
      break;
    }
    std::cout << "received server : " << receiveMessage << std::endl;
  }
  close(clientSocket);
  return 0;
}