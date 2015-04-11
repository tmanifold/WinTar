#ifndef _TARFILE_H_
#define _TARFILE_H_

#define PATH_MAX 1024

#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <direct.h>
#include "TarHeader.h"

using std::vector;
using std::ios;

class TarFile{
private:
    char *file_name;
    char *full_path;
    long  file_size;
    char *buff;
    long  buff_size;

    TarHeader *_head;

    vector<TarHeader> headers;

    std::ifstream read_stream;
    std::ofstream write_stream;

    int otoi(char *, unsigned int);
    int round512(int);
    int check_block(char *, int);

public:
    TarFile(char *n);
    TarFile(vector<char *> &files);
    ~TarFile();

    void read_head();
    void untar();
    void list_tar();
    void write_tar();
};

#endif // _TARFILE_H_
