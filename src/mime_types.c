#include "mime_types.h"
#include <string.h>

const char* detect_content_type(const char* filename){
    const char* extension = strrchr(filename, '.');
    
    if(extension == NULL) {
       return "application/octet-stream";
    }
    if(strcmp(extension, ".html") == 0 || strcmp(extension, ".htm") == 0) {
       return "text/html; charset=utf-8";
    }
    else if (strcmp(extension, ".js") == 0) {
       return "application/javascript; charset=utf-8";
    }
    else if (strcmp(extension, ".json") == 0) {
       return "application/json; charset=utf-8";
    }
    else if (strcmp(extension, ".txt") == 0) {
       return "text/plain; charset=utf-8";
    }
    else if (strcmp(extension, ".png") == 0) {
       return "image/png";
    }
    else if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0){
       return  "image/jpeg";
    }
    else if (strcmp(extension, ".gif") == 0) {
       return "image/gif";
    }
    else if (strcmp(extension, ".pdf") == 0) {
       return "application/pdf";
    }
  
    return "application/octet-stream";
}
