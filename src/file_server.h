#ifndef FILE_SERVER_H
#define FILE_SERVER_H

#include <stddef.h>

typedef enum {
    FILE_OK = 0,
    FILE_NOT_FOUND = 404,
    FILE_FORBIDDEN = 403,
    FILE_ERROR = 500
} FileStatus;

typedef struct {
    char * content;
    size_t size;
    FileStatus status;
    const char* mime_type;
} FileInfo;

FileInfo* load_file(const char* filepath);
void free_file_info(FileInfo* info);
int is_path_safe(const char* path);

#endif

