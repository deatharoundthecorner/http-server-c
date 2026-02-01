#include "http_handler.h"
#include "file_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int parse_http_request(const char* raw_request, HttpRequest* request) {
    int parsed = sscanf(raw_request, "%15s %511s %15s",
                        request->method, request->uri, request->version);
    return (parsed == 3) ? 0 :-1;
}

void send_http_response(int socket_fd, int status_code, const char* content_type, const char* body,size_t body_size){
    char header[HTTP_RESPONSE_HEADER_MAX_SIZE];
    const char* status_text;
 
    switch (status_code) {
        case 200: status_text = "ok"; break;
        case 400: status_text = "Bad Request"; break;
        case 403: status_text = "Forbidden"; break;
        case 404: status_text = "Not Found"; break;
        case 500: status_text = "Internal Server Error"; break;
        default: status_text = "unknown"; break;
    }

int header_len = snprintf(header, sizeof(header),
     "HTTP/1.1 %d %s\r\n"
     "Content-Type: %s\r\n"
     "Content-Length: %zu\r\n"
     "Connection: close\r\n"
     "Server: CustomHTTPServer/1.0\r\n"
     "\r\n",
     status_code, status_text, content_type, body_size);

     send(socket_fd, header, header_len,0);

     if (body && body_size > 0) {
          send(socket_fd, body, body_size,0);
     }
}

void send_error_page(int socket_fd, int status_code, const char* message) {
    char error_html[512];
    int body_len = snprintf(error_html, sizeof(error_html),
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head><title>Error %d</title></head>\n"
        "<body>\n"
        "<h1>Error %d</h1>\n"
        "<p>%s</p>\n"
        "</body>\n"
        "</html>\n",
        status_code, status_code, message);
    
    send_http_response(socket_fd, status_code, "text/html", error_html, body_len);
}

void handle_client_connection(int client_fd, const char* web_root) {
    char request_buffer[HTTP_REQUEST_MAX_SIZE];
    
    ssize_t bytes_received = recv(client_fd, request_buffer, 
                                  sizeof(request_buffer) - 1, 0);
    
    if (bytes_received <= 0) {
        close(client_fd);
        return;
    }
    
    request_buffer[bytes_received] = '\0';

    HttpRequest request;
    if (parse_http_request(request_buffer, &request) != 0) {
        send_error_page(client_fd, 400, "Malformed HTTP request");
        close(client_fd);
        return;
    }
    
    if (strcmp(request.method, "GET") != 0) {
        send_error_page(client_fd, 400, "Only GET method is supported");
        close(client_fd);
        return;
    }

    char filepath[1024];
    if (strcmp(request.uri, "/") == 0) {
        snprintf(filepath, sizeof(filepath), "%s/index.html", web_root);
    } else {
        snprintf(filepath, sizeof(filepath), "%s%s", web_root, request.uri);
    }

    FileInfo* file_info = load_file(filepath);
    
    if (!file_info) {
        send_error_page(client_fd, 500, "Internal server error");
        close(client_fd);
        return;
    }

    switch (file_info->status) {
        case FILE_OK:
            send_http_response(client_fd, 200, file_info->mime_type, 
                             file_info->content, file_info->size);
            printf("Served: %s (%zu bytes)\n", filepath, file_info->size);
            break;
            
        case FILE_NOT_FOUND:
            send_error_page(client_fd, 404, "The requested file was not found");
            printf("404: %s\n", filepath);
            break;
            
        case FILE_FORBIDDEN:
            send_error_page(client_fd, 403, "Access to this file is forbidden");
            printf("403: %s\n", filepath);
            break;
            
        case FILE_ERROR:
        default:
            send_error_page(client_fd, 500, "Error reading file");
            printf("500: %s\n", filepath);
            break;
    }
    
    free_file_info(file_info);
    close(client_fd);
}
