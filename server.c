#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 2000
#define BACKLOG 10


int main(){
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socketFD == -1){
        perror("socket");
        return -1;
    } else{
        printf("Socket created.\n");
    }

    struct sockaddr_in SVR_address = {0};
    SVR_address.sin_family = AF_INET;
    SVR_address.sin_addr.s_addr = INADDR_ANY;
    SVR_address.sin_port = htons(PORT);

    if(bind(socketFD, (struct sockaddr *)&SVR_address, sizeof(SVR_address)) == -1){
        perror("bind");
        close(socketFD);
        return -1;
    }

    if(listen(socketFD, BACKLOG) == -1){
        perror("listen");
        close(socketFD);
        return -1;
    }

    struct sockaddr_in USR_address = {0};

    socklen_t sizeUsrAddr = sizeof(USR_address);

    int usrFD = accept(socketFD, (struct sockaddr *) &USR_address, &sizeUsrAddr);

    if(usrFD < 0){
        perror("accept");
        close(socketFD);
        return -1;
    }

    char buff[1024];

    ssize_t message = recv(usrFD, &buff, sizeof(buff) - 1, 0);

    if(message < 0){
        perror("recv");
        close(usrFD);
        close(socketFD);
        return -1;
    }

    buff[message] = '\0';

    printf("%s\n", buff);

    char svrBuff[1024] = "Hello Client!\0";

    ssize_t messageBack = send(usrFD, &svrBuff, strlen(svrBuff), 0);

    if(messageBack < 0){
        perror("send");
        close(usrFD);
        close(socketFD);
    }

    close(usrFD);
    close(socketFD);
}