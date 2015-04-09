
#include "TarFile.h"
#include <cstdlib>
#include <cstring>

TarFile::TarFile(char *n) {
    this->file_stream.open(n, ios::in | ios::binary);

    if (file_stream.is_open()) {
        this->file_name = n;
        //GetFullPathName(n, 256, full_path, NULL);
        file_stream.seekg(0, ios::end);

        this->file_size = file_stream.tellg();
        this->buff_size = this->file_size - 5120;

        file_stream.seekg(0, ios::beg);

        this->buff = (char *) malloc(buff_size);

        printf("\n%s: %d bytes\n", this->file_name, this->file_size);
        printf("buffer: %d bytes\n\n", buff_size);

        this->read_head();

    } else {
        printf("Could not open files: %s\n", n);
    }
}

TarFile::~TarFile() {
    if (file_stream.is_open()) {
        this->file_stream.close();
    }
}

// list the contents of a tar file
void TarFile::list_tar() {
    printf("name\t\tsize (bytes)\n");
    for (int h = 0; h < headers.size(); h++) {
        printf("%s\t\t%d\n", headers[h].fname, headers[h].fsize);
    }
}

// Read the file headers and store a TarHeader object in the headers vector for each entry.
void TarFile::read_head() {
    file_stream.read(buff, buff_size);
    file_stream.close();
    int pos = 0;
    int byte_count = 0;
    long bytes_left = buff_size;

    while (byte_count < buff_size && check_block(buff, pos) < 512) {

        _head = TarHeader();
        printf("pos: %d\n", pos);
        for (int i = 0; i < HBLK_SIZE; i++) {
            _head.buffer[i] = buff[pos + i];
        }
        for (int i = 0; i < 100; i++) {
            _head.fname[i] = _head.buffer[i];
        }
        _head.link = _head.buffer[156];

        if (_head.link == '5') { // the entry is a directory
            _head.is_dir = true;
            _head.fsize = 0;
            _head.start = pos;
            pos += HBLK_SIZE;
            byte_count += HBLK_SIZE;
        } else {
            _head.is_dir = false;
            _head.fsize = otoi(&_head.buffer[124], 11);
            _head.start = pos;
            pos += round512(_head.fsize) + HBLK_SIZE;
            byte_count += HBLK_SIZE + round512(_head.fsize);
        }

        headers.push_back(_head);
        printf("fname: %s\nfsize: %d\nlink: %c\nbyte_count: %d\n\n", _head.fname, _head.fsize, _head.link, byte_count);


        //if (buff[pos] == '\0') { null_block_found = true; }
    }

}

// checks for consecutive null blocks in the buffer
//  returns number of null bytes in the block
int TarFile::check_block(char *blk, int p) {

    int null_bytes = 0;

    for (int byte = 0; byte < HBLK_SIZE * 2; byte++) {
        if (blk[p + byte] == '\0') {
            null_bytes++;
        } else {
            null_bytes = 0;
        }
    }
    return null_bytes;
}

// convert octal filesize block in header to int
int TarFile::otoi(char *curr_ch, unsigned int size) {
    unsigned int output = 0;
    while (size > 0) {
        output = output * 8 + *curr_ch - '0';
        curr_ch++;
        size--;
    }
    return output;
}

// rounds to nearest multiple of 512
int TarFile::round512(int n) {
    int rounded;

    if (n <= HBLK_SIZE) {
        rounded = HBLK_SIZE;
    } else {
        rounded = (n + HBLK_SIZE) - (n % HBLK_SIZE);
    }
    return rounded;
}

void TarFile::untar() {
    //read_head();
    for (int i = 0; i < headers.size(); i++) {

        long data_block_size = round512(headers[i].fsize);
        char *data_block = (char *) malloc(data_block_size);
        this->write_stream.open(headers[i].fname, ios::out | ios::binary);

        printf("opened %s\n", headers[i].fname);

        for (int byte = 0; byte < data_block_size; byte++) {
            data_block[byte] = buff[byte + headers[i].start + HBLK_SIZE];
        }

        this->write_stream.open(headers[i].fname, ios::out);
        printf("opened %s\n", headers[i].fname);

        for (int byte = 0; byte < headers[i].fsize; byte++) {
            data_block[i] = buff[i + headers[i].start + 513];
        }
        if (!headers[i].is_dir) {
            write_stream.write(data_block, headers[i].fsize);
        } else {
            CreateDirectory(headers[i].fname, NULL);
        }

        printf("\tdata_block_size: %d\n", data_block_size);

        write_stream.close();
    }
}
