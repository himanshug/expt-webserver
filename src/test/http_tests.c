#include "utils.h"
#include "http.h"
#include "minunit.h"

#include <fcntl.h>

int main(int argc, int *argv[]) {
    /*
    char *test_msg = "GET /xyz HTTP/1.0\r\n"
            "Host: localhost:4080\r\n"
            "Connection: Keep-Alive\r\n\r\n";
            */

    int fd = open("res/test/simple_get_msg", O_RDONLY);
    if(fd < 0) die(1, "couldnt open test msg file");
    http_msg *msg = read_http_msg(fd);

    mu_assert(strcmp(msg->method,"GET") == 0, "method parsing failed");
    mu_assert(strcmp(msg->path,"/xyz") == 0, "path parsing failed");
    mu_assert(strcmp(msg->ver,"1.0") == 0, "version parsing failed");
}
