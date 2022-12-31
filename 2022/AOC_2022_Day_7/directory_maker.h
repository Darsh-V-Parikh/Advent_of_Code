#ifndef _DIRECTORY_MAKER_H_
#define _DIRECTORY_MAKER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct __Directory;

enum content_t {
    cDIRECTORY   = 0,
    cFILE        = 1,
};

typedef union {
    struct __Directory *dir_pntr;
    struct __File *file_pntr;
} content_val_t;

struct __DirContent{
    std::string content_name;
    content_t content_id;
    content_val_t content_data;
};

struct __Directory {
    std::string dir_name;
    struct __Directory *parent_dir;
    std::vector<struct __DirContent> dir_contents;
    size_t dir_size;
};

struct __File {
    size_t size;
    struct __Directory *parent_dir;
};

typedef struct __DirContent DirContent;
typedef struct __Directory Directory;
typedef struct __File File;

File* makeNewFile(size_t file_size, Directory *parent_pntr);
void deleteFile(File *file);
Directory* makeNewDirectory(std::string dir_name, Directory *parent_pntr);
void deleteDirectory(Directory *dir);
void addNewContent(Directory *curr_dir, std::string content_name, content_t content_id, size_t file_size=0);
void fileAccounting(Directory *dir, size_t size);

#endif