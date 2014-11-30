#ifndef HTTP_H_
#define HTTP_H_

struct http_msg {
    char *method; //TODO: correct it to be enum
    char *path;
    char *ver;
};
typedef struct http_msg http_msg;

http_msg *read_http_msg(int fd);

void free_http_msg(http_msg *msg);

#endif /* HTTP_H_ */
