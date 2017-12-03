#include <stdio.h>
#include <sys/malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>

#define PORT 8181 

#define LIP  "127.0.0.1"
#define BUF_LEN 1024

int main()
{
    int fd, nfd;
    socklen_t len;
    int nbyte_read;
    char buffer[BUF_LEN] = {0};

    struct sockaddr_in sockaddr, client_addr;

    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(PORT);
    sockaddr.sin_addr.s_addr = inet_addr(LIP);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    int option = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)) < 0)
    { 
        perror("set reuse socket");
        exit(1);
    }

    if ( bind(fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0 ) {
        perror("bind");
        exit(1);
    }

    if ( listen(fd, 20) < 0 ) {
        perror("listen");
        exit(1);
    }

    len = sizeof(client_addr);

    while(1) {
        nfd = accept(fd, (struct sockaddr*)&client_addr,&len);

        if ( nfd < 0 ) {
            perror("accept");
            exit(1);
        }
        
        nbyte_read = recv(nfd, buffer, BUF_LEN, 0);

        if ( nbyte_read < 0 ) {
            perror("recv");
            exit(1);
        }

        buffer[nbyte_read] = '\0';

        fprintf(stdout, "%s\n", buffer);

        sleep(5);

        if ( send(nfd, buffer, nbyte_read, 0) < 0 ) {
            perror("send");
            exit(1);
        }
    }

    close(fd);

    return 0;
}
