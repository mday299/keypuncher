#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#define MAX 500
#define port 5200

int main() {
  int serverSocketHandler = socket(AF_INET, SOCK_STREAM, 0);
  // creating a socket and assigning it to the socket handler
  if (serverSocketHandler < 0) {
    // socket methode return -1 if the creation was not successful
    std::cout << "Socket creation has failed.";
    return 0;
  }
  struct sockaddr_in serverAddr, clientAddr;
  // specifying address and type for the server to operate under
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  int bindStatus = bind(serverSocketHandler, (struct sockaddr *)&serverAddr,
                        sizeof(serverAddr));
  if (bindStatus < 0) {
    std::cout << "Socket binding has failed" << std::endl;
    return 0;
  }
  // listen to the client while others are waiting in queue of size 5
  int listenStatus = listen(serverSocketHandler, 5);
  if (listenStatus < 0) {  // when queue is full listen fails
    std::cout << "Listner has failed" << std::endl;
    return 0;
  }
  std::cout << "\t\t...Waiting for connections... \n\n";
  char buff[MAX];
  int clientSocketHandler;
  socklen_t len = sizeof(clientAddr);
  int connection;
  if ((connection = accept(serverSocketHandler, (struct sockaddr *)&clientAddr,
                           &len)) < 0) {
    std::cout << "Server didn't accept the request." << std::endl;
    return 0;
  } else {
    std::cout << "Server accepted the request. \n";
  }

  while (true) {
    // infinite loop for chatting
    int rMsgSize;

    if ((rMsgSize = recv(connection, buff, MAX, 0)) > 0) {
      std::cout << "received client : " << buff << std::endl;

      if (buff[0] == 'b' && buff[1] == 'y' && buff[2] == 'e') {
        std::cout << "Server : Bye bro" << std::endl;
        std::cout << "\nConnection ended... take care bye bye...\n";
        send(connection, buff, strlen(buff) + 1, 0);
        break;
      }

      std::cout << "Server : ";
      char input[MAX];
      std::string s;
      getline(std::cin, s);
      int n = s.size();
      for (int i = 0; i < n; i++) {
        input[i] = s[i];
      }

      input[n] = '\0';

      send(connection, input, strlen(input) + 1, 0);
    }
  }
  close(serverSocketHandler);
  return 0;
}