#include <errno.h> //errno
#include <stdlib.h> //NULL
#include <stdio.h> //perror

#include <sys/socket.h> //socket functions
#include <netinet/in.h> //socket address structure

#include "utils.h"
#include "http.h"

#define PORT_NUM 4080
#define BUFFER_SIZE 4096

static const int STD_OUT = 1;

static char buff[BUFFER_SIZE];

static void write_resp_to_fd(char *path, int fd) {
    FILE *tmp_fd = fopen(path, "rb");

    if(tmp_fd == NULL) {
        dprintf(fd, "HTTP/1.0 %d %s\r\n\r\n", 404, "Resource not found.");
    } else {
        fseek(tmp_fd, 0, SEEK_END);
        int len = ftell(tmp_fd);
        fseek(tmp_fd, 0, SEEK_SET);

        dprintf(fd, "HTTP/1.0 %d %s\r\n", 200, "OK");
        dprintf(fd, "Content-Length: %d\r\n\r\n", len);
        while(1) {
            int len = fread(buff,1,BUFFER_SIZE,tmp_fd);
            if(len > 0) {
                write(fd,buff,len);
            } else break;
        }
        fclose(tmp_fd);
    }
}
int main(int argc, char *argv[]) {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) die(1,"couldn't create socket");

    struct sockaddr_in name;
    name.sin_family = AF_INET;
    name.sin_port = htons(PORT_NUM);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind (fd, (struct sockaddr *)&name, sizeof(name)) < 0) die(1,"coundn't bind socket");

    if(listen(fd, 2) < 0) die(1,"couldn't make the server socket");

    //read message and just print it

    for(;;) {
        printf("Waiting for new connections.\n");
        int cfd = accept(fd, NULL, NULL);
        if(cfd < 0) die(1,"failed to accept connection");

        http_msg *msg = read_http_msg(cfd);

        buff[0] = '\0';
        strcat(buff,"/tmp/www");
        strcat(buff,msg->path);
        write_resp_to_fd(buff, cfd);
        if(close(cfd) < 0) die(1,"failed to close conn");
    }
}
