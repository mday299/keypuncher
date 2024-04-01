#include <iostream>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define MAX_BUFFER 1024

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    const char *message;
    char server_reply[MAX_BUFFER];
    int recv_size;

    std::cout << "Initialising Winsock...";
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        std::cout << "Failed. Error Code : " << WSAGetLastError();
        return 1;
    }

    std::cout << "Initialised.\n";

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        std::cout << "Could not create socket : " << WSAGetLastError();
        return 1;
    }

    std::cout << "Socket created.\n";

    server.sin_addr.s_addr = inet_addr("93.184.216.34"); // example.com's IP
    server.sin_family = AF_INET;
    server.sin_port = htons(80); // HTTP Port

    //Connect to remote server
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cout << "connect error";
        return 1;
    }

    std::cout << "Connected\n";

    //Send some data
    message = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";
    if (send(s, message, strlen(message), 0) < 0) {
        std::cout << "Send failed";
        return 1;
    }
    std::cout << "Data Send\n";

    //Receive a reply from the server
    while((recv_size = recv(s, server_reply, MAX_BUFFER-1, 0)) > 0) {
        server_reply[recv_size] = '\0';
        std::cout << server_reply;
    }

    if(recv_size < 0) {
        std::cout << "recv failed";
        return 1;
    }

    // Cleanup and close the connection
    closesocket(s);
    WSACleanup();

    return 0;
}
