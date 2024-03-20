#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define MAX_BUFFER 1024

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message, server_reply[MAX_BUFFER];
    int recv_size;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }

    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf("Could not create socket : %d" , WSAGetLastError());
        return 1;
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("93.184.216.34"); // example.com's IP
    server.sin_family = AF_INET;
    server.sin_port = htons(80); // HTTP Port

    //Connect to remote server
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("connect error");
        return 1;
    }

    puts("Connected");

    //Send some data
    message = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
    if (send(s, message, strlen(message), 0) < 0) {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");

    //Receive a reply from the server
    if((recv_size = recv(s, server_reply, MAX_BUFFER, 0)) == SOCKET_ERROR) {
        puts("recv failed");
        return 1;
    }

    puts("Reply received\n");
    server_reply[recv_size] = '\0';
    puts(server_reply);

    // Cleanup and close the connection
    closesocket(s);
    WSACleanup();

    return 0;
}