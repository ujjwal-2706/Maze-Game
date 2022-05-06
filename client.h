// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#define PORT 8080

int sock = 0, valread2;
struct sockaddr_in serv_addr;

int makeClientSide()
{
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "10.184.28.152", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if (connect(sock, (struct sockaddr*)&serv_addr,
                sizeof(serv_addr))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}

int sendToServer(const char *message)
{
    char buffer2[1024] = { 0 };
    send(sock, message, strlen(message), 0);
    return 0;
}

string readFromServer()
{
    char buffer2[1024] = { 0 };
    valread2 = read(sock, buffer2, 1024);
    return buffer2;
}