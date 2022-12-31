#include "directory_maker.h"

#define MAX_MEM_SIZE 70000000
#define MEM_NEEDED   30000000
#define DIR_SIZE_CAP 100000
Directory *TopDir = NULL, *CurrDir = NULL;

void changeCurrDir(std::string dir_name) {
    if (TopDir == NULL) {
        TopDir = makeNewDirectory(dir_name, NULL);
        CurrDir = TopDir;
    }
    else if (dir_name == "..") {
        if (CurrDir->parent_dir) {
            CurrDir = CurrDir->parent_dir;
        }
    }
    else {
        for (auto c : CurrDir->dir_contents) {
            if ((c.content_id == cDIRECTORY) && (c.content_name == dir_name)) {
                CurrDir = c.content_data.dir_pntr;
            }
        }
    }
}

void commandLineParser(std::ifstream *cl_file) {
    std::string type, name;
    std::stringstream command;
    while (std::getline(*cl_file, type)) {
        command.str(type);
        command >> type >> name;
        if (type == "$") {
            if (name == "cd") {
                command >> name;
                changeCurrDir(name);
            }
        }
        else if (type == "dir") {
            addNewContent(CurrDir, name, cDIRECTORY);
        }
        else {
            addNewContent(CurrDir, name, cFILE, (size_t)(atoi(type.c_str())));
        }
        type.clear();
        name.clear();
        command.clear();
    }
}

void showDirStructure(Directory *dir, int top_offset);

size_t _findDirectoriesAndSize(Directory *dir, size_t cap) {
    size_t t = 0;
    for (DirContent c : dir->dir_contents) {
        if (c.content_id == cDIRECTORY) {
            t += _findDirectoriesAndSize(c.content_data.dir_pntr, cap);
        }
    }
    if (dir->dir_size <= cap) {
        std::cout << "\t\""  << dir->dir_name << "\" : " << dir->dir_size << std::endl;
        t += dir->dir_size;
    }
    return t;
}

void findDirectoriesUnderSize(Directory *top_dir, size_t size_cap) {
    std::cout << std::endl << "Directories whose size is within " << size_cap << std::endl;
    size_t t =_findDirectoriesAndSize(top_dir, size_cap);
    std::cout << "Total Size: " << t << std::endl;
}

void _findDeletableDirectory(Directory *dir, size_t min, Directory **smallest) {
    for (DirContent c : dir->dir_contents) {
        if (c.content_id == cDIRECTORY) {
            _findDeletableDirectory(c.content_data.dir_pntr, min, smallest);
        }
    }
    if ((dir->dir_size >= min) && (dir->dir_size < (*smallest)->dir_size)) {
        (*smallest) = dir;
    }
}

void findDeletableDirectory(Directory *start_dir, size_t min_space_needed) {
    Directory *smallest_deletable = start_dir;
    _findDeletableDirectory(start_dir, min_space_needed, &smallest_deletable);
    std::cout << std::endl << "Best one to delete is \"" << smallest_deletable->dir_name
              << "\" which frees up " << smallest_deletable->dir_size;
}

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream dir_data(argv[1], std::ios::in);
    if (!dir_data.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    commandLineParser(&dir_data);
    showDirStructure(TopDir, 0);
    findDirectoriesUnderSize(TopDir, DIR_SIZE_CAP);
    findDeletableDirectory(TopDir, MEM_NEEDED-(MAX_MEM_SIZE-TopDir->dir_size));

    deleteDirectory(TopDir);
    return 0;
}

// ----------------------------------------------------------------------------

File* makeNewFile(size_t file_size, Directory *parent_pntr) {
    File *new_file = new File;
    new_file->parent_dir = parent_pntr;
    new_file->size = file_size;
    return new_file;
}

void deleteFile(File *file) {
    delete file;
}

Directory* makeNewDirectory(std::string dir_name, Directory *parent_pntr) {
    Directory *new_dir = new Directory;
    new_dir->dir_name = dir_name;
    new_dir->dir_contents.clear();
    new_dir->parent_dir = parent_pntr;
    new_dir->dir_size = 0;
    return new_dir;
}

void deleteDirectory(Directory *dir) {
    for (DirContent c : dir->dir_contents) {
        if (c.content_id == cDIRECTORY) {
            deleteDirectory(c.content_data.dir_pntr);
        }
        else if (c.content_id == cFILE) {
            deleteFile(c.content_data.file_pntr);
        }
    }
    delete dir;
}

void addNewContent(Directory *curr_dir, std::string content_name, content_t content_id, size_t file_size) {
    DirContent new_content;
    new_content.content_name = content_name;
    switch (content_id) {
    case cDIRECTORY:
        new_content.content_data.dir_pntr = makeNewDirectory(content_name, curr_dir);
        break;
    case cFILE:
        new_content.content_data.file_pntr = makeNewFile(file_size, curr_dir);
        fileAccounting(curr_dir, file_size);
        break;
    default:
        fprintf(stdout, "weird content id %d for %s in %s", content_id, content_name, curr_dir->dir_name);
        break;
    }
    new_content.content_id = content_id;
    curr_dir->dir_contents.push_back(new_content);
}

void fileAccounting(Directory *dir, size_t size) {
    while (dir) {
        dir->dir_size += size;
        dir = dir->parent_dir;
    }
}

// ----------------------------------------------------------------------------

void showDirStructure(Directory *dir, int top_offset) {
    std::string offset_str = "";
    for (int i = 0; i < top_offset; i++) {
        offset_str += " ";
    }

    std::cout << offset_str << " - " << dir->dir_name 
              << " (dir, size=" << dir->dir_size << ")" << std::endl;
    for (DirContent c : dir->dir_contents) {
        if (c.content_id == cDIRECTORY) {
            showDirStructure(c.content_data.dir_pntr, top_offset+1);
        }
        else if (c.content_id == cFILE) {
            std::cout << offset_str + "   - " << c.content_name 
                      << " (file, size=" << c.content_data.file_pntr->size << ")" << std::endl;
        }
    }
}