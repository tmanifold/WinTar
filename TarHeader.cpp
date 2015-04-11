
#include "TarHeader.h"
#include <cstdlib>

TarHeader TarHeader::clone() {
    TarHeader _head;
    strcpy(_head.buffer,    this->buffer);
    strcpy(_head.checksum,  this->checksum);
    strcpy(_head.fmode,     this->fmode);
    strcpy(_head.fname,     this->fname);
    strcpy(_head.group_id,  this->group_id);
    strcpy(_head.last_mod,  this->last_mod);
    strcpy(_head.link,      this->link);
    strcpy(_head.link_name, this->link_name);
    strcpy(_head.owner_id,  this->owner_id);

    _head.fsize     = this->fsize;
    _head.is_dir    = this->is_dir;
    _head.start     = this->start;

    return _head;
}
