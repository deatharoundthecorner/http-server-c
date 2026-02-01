#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include<stddef.h>

#define HTTP_REQUEST_MAX_SIZE 8192
#define HTTP_RESPONSE_HEADER_MAX_SIZE 1024

typedef struct {
    char method[16];
    char uri[512];
    char version[16];
} HttpRequest;

int parse_http_request(const char* raw_request,HttpRequest* request);
void send_http_response(int socket_fd, int status_code, const char* content_type,const char* body, size_t body_size);
void send_error_page(int socket_fd, int status_code, const char* message);
void handle_client_connection(int client_fd, const char* web_root);

#endif

