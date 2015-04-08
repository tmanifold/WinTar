
#include "TarHeader.h"
#include <cstdlib>

TarHeader::TarHeader() {
    fsize = (int) malloc(12);

}

void TarHeader::destroy() {
    delete buffer;
    delete fname;
    delete fmode;
    delete owner_id;
    delete group_id;
    fsize = NULL;
    delete last_mod;
    delete checksum;
    delete link;
    delete link_name;
    start = NULL;
}
