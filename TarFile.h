#ifndef _TARFILE_H_
#define _TARFILE_H_

#include <fstream>
#include <iostream>
#include <vector>
#include "TarHeader.h"

using std::vector;
using std::ios;

class TarFile{
private:
    char *fname;
    char *buff;
    int buff_size;
    TarHeader _head;

    vector<TarHeader> headers;
    std::fstream file_stream;
    std::fstream write_stream;

    int otoi(char *, unsigned int);
    int round512(int);
public:
    TarFile(char *n);
    ~TarFile();
    void read_head();
    void untar();
};

#endif // _TARFILE_H_
