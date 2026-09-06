#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(void){

    int socketFD = socket(/*domain (ips4)*/ AF_INET, /*type (tcp)*/SOCK_STREAM, /*protocol*/0);

    if(socketFD == -1){
        perror("Error opening socket\n");
        return -1;
    } else{
        printf("Successfully created socket :)\n");
    }

    struct sockaddr_in address = {0};

    char *ip = "127.0.0.1";

    address.sin_port = htons(2000);
    address.sin_family = AF_INET;
    if(inet_pton(AF_INET, ip, &address.sin_addr.s_addr) != 1){
        perror("inet_pton\n");
        close(socketFD);
        return -1;
    }

    int result = connect(socketFD, /*I don't know why this works, but it does*/(struct sockaddr *)&address, sizeof(address));

    if(result == -1){
        perror("Error connecting\n");
        close(socketFD);
        return -1;
    } else{
        printf("successfully connected :)\n");
    }

    char *message;

    message = "Hello Server!\n";

    int bytesSent = send(socketFD, message, strlen(message), 0);

    if(bytesSent == -1){
        perror("send");
        close(socketFD);
        return -1;
    }

    char buffer[1024];

    int bytesRecv = recv(socketFD, buffer, sizeof(buffer) - 1, 0);

    if(bytesRecv == -1){
        perror("recv");
        close(socketFD);
        return -1;
    }

    printf("%s\n", buffer);

    close(socketFD);

    return 0;
}