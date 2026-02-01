#include "file_server.h"
#include "mime_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int is_path_safe(const char* path) {
    if (strstr(path, "..") != NULL) {
        return 0;
    }
    return 1;
}

FileInfo* load_file(const char* filepath) {
    FileInfo* info = malloc(sizeof(FileInfo));
    if (!info) {
       return NULL;
    }
    
    info->content = NULL;
    info->size = 0;
    info->status = FILE_ERROR;
    info->mime_type = detect_content_type(filepath);

    if (!is_path_safe(filepath)) {
        info->status = FILE_FORBIDDEN;
        return info;
    }
    
    struct stat file_stat;
    if(stat(filepath, &file_stat) !=0) {
       info->status = FILE_NOT_FOUND;
       return info;
    }

    if (S_ISDIR(file_stat.st_mode)) {
       info->status = FILE_FORBIDDEN;
       return info;
    }

    FILE* fp = fopen(filepath, "rb");
    if (!fp) {
         info->status = FILE_NOT_FOUND;
         return info;
    }
    
    info->size = file_stat.st_size;
    info->content = malloc(info->size);
    if(!info->content) {
       fclose(fp);
       info->status = FILE_ERROR;
       return info;
    }

    size_t bytes_read = fread(info->content, 1, info->size, fp);
    fclose(fp);
    
    if (bytes_read != info->size) {
        free(info->content);
        info->content = NULL;
        info->status = FILE_ERROR;
        return info;
    }
   
    info->status = FILE_OK;
    return info;
    }
    void free_file_info(FileInfo* info) {
      if (info) {
         if (info->content) {
             free(info->content);
         }
         free(info);
    }
}
