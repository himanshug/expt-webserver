#include "http.h"
#include "utils.h"

#include <string.h>

#define BUFF_SIZE 4096

char buffer[BUFF_SIZE];

static char SP = ' ';
static char CR = "\r";

static int read_char(int fd, char *ch) {
    int n = read(fd, &ch, 1);
    if(n < 0) die(1, "couldn't read from rd");
    return n; //should always be 1
}

static int read_token(int fd, char *buffer, int max_len, char terminal) {
    int i = 0;
    char ch;
    do {
        read_char(fd, &ch);
        buffer[i++] = ch;
    } while(i < max_len && ch != terminal);

    if(i >= max_len) die(2, "token len exceeded limit");
    return i;
}

http_msg *read_http_msg(int fd) {
    http_msg *msg = malloc(sizeof(http_msg));

    int len = read_token(fd, buffer, BUFF_SIZE, SP);
    msg->method = strndup(buffer,len);

    len = read_token(fd, buffer, BUFF_SIZE, SP);
    msg->path = strndup(buffer,len);

    len = read_token(fd, buffer, BUFF_SIZE, '/');
    if(len != 4) die("expected HTTP here");

    len = read_token(fd, buffer, BUFF_SIZE, CR);
    msg->ver = strndup(buffer,len);

    //TODO:read and ignore everything till the end
    return msg;
}


