#include <errno.h> //errno
#include <stdlib.h> //NULL
#include <stdio.h> //perror

#include <sys/socket.h> //socket functions
#include <netinet/in.h> //socket address structure

#include "utils.h"

#define PORT_NUM 4080
#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) die("couldn't create socket");

    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(PORT_NUM);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind (fd, (struct sockaddr *)&name, sizeof(name)) < 0) die("coundn't bind socket");

    if(listen(fd, 2) < 0) die("couldn't make the server socket");

    //read message and just print it
    char buff[BUFFER_SIZE];
    for(;;) {

        printf("Waiting for new connections.\n");
        int cfd = accept(fd, NULL, NULL);
        if(cfd < 0) die("failed to accept connection");

        while(1) {
            read_http_message(cfd);
            int len = read(cfd, buff, sizeof(buff));
            if(len > 0) printf(buff);
            else if(len == 0) {
                if(close(cfd) < 0) die("failed to close conn");
                break;
            } else {
                die("failed to read data");
            }
        }
    }
}
