
#ifndef _TARHEADER_H_
#define _TARHEADER_H_

class TarHeader {
private:
    char buffer[512];
    char fname[100];
    char fmode[8];
    char owner_id[8];
    char group_id[8];
    int fsize;
    char last_mod[12];
    char checksum[8];
    char link[1];
    char link_name[100];
    int start;
    friend class TarFile;
};

#endif // _TARHEADER_H_
