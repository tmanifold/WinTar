
#ifndef _TARHEADER_H_
#define _TARHEADER_H_

#define HBLK_SIZE 512

#include <cstring>

class TarHeader {
private:
    char buffer[HBLK_SIZE];
    char fname[100];
    char fmode[8];
    char owner_id[8];
    char group_id[8];
    int  fsize;
    char last_mod[12];
    char checksum[8];
    char link[1];
    char link_name[100];
    int  start;

    bool is_dir;
    friend class TarFile;
    friend class TarHeader;
public:
    TarHeader clone();
};

#endif // _TARHEADER_H_
