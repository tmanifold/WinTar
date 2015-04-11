
#include "TarFile.h"
#include <cstdlib>
#include <cstring>

TarFile::TarFile(char *n) {

    this->read_stream.open(n, ios::in | ios::binary);

    if (read_stream.is_open()) {
        this->file_name = n;
        //GetFullPathName(n, 256, full_path, NULL);
        read_stream.seekg(0, ios::end);

        this->file_size = read_stream.tellg();
        this->buff_size = this->file_size - 5120;

        read_stream.seekg(0, ios::beg);

        this->buff = (char *) malloc(buff_size);

        printf("\n%s: %d bytes\n", this->file_name, this->file_size);
        printf("buffer: %d bytes\n\n", buff_size);

        this->read_head();

    } else {
        printf("Could not open files: %s\n", n);
    }
}

TarFile::TarFile(vector<char *> &files) {

    for (int i = 0; i < files.size(); i++) {
        // get file information and create headers,
        // then write to new archive
    }
}

TarFile::~TarFile() {

    if (read_stream.is_open()) {

        this->read_stream.close();
    }
    if (write_stream.is_open()) {

        this->write_stream.close();
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

    read_stream.read(buff, buff_size);
    read_stream.close();

    int pos        = 0;
    int byte_count = 0;

    while (byte_count < buff_size && check_block(buff, pos) < 1024) {

        _head = new TarHeader();
 //       printf("pos: %d\n", pos);
        for (int i = 0; i < HBLK_SIZE; i++) {

            _head->buffer[i] = buff[pos + i];
        }
        for (int i = 0; i < 100; i++) {

            _head->fname[i] = _head->buffer[i];
        }

        _head->link[0] = _head->buffer[156];

        if (_head->link[0] == '5') { // the entry is a directory

            _head->is_dir = true;
            _head->fsize = 0;
            _head->start = pos;

            pos        += HBLK_SIZE;
            byte_count += HBLK_SIZE;

        } else {

            _head->is_dir = false;
            _head->fsize = otoi(&_head->buffer[124], 11);
            _head->start = pos;

            pos        += HBLK_SIZE + round512(_head->fsize);
            byte_count += HBLK_SIZE + round512(_head->fsize);
        }

        headers.push_back(_head->clone());
 //       printf("fname: %s\nfsize: %d\nlink: %c\nbyte_count: %d\n\n", _head.fname, _head.fsize, _head.link, byte_count);
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

    for (int i = 0; i < headers.size(); i++) {

        char file_path[PATH_MAX];
        char dir[PATH_MAX];

        long data_block_size = round512(headers[i].fsize);
        char *data_block     = (char *) malloc(data_block_size);

        _getcwd(file_path, PATH_MAX);

        strcpy(dir, file_path);
        strcat(file_path, "\\");
        strcat(file_path, headers[i].fname);

        this->write_stream.open(file_path, ios::out | ios::binary);

//        printf("opened %s\n", headers[i].fname);

        for (int byte = 0; byte < data_block_size; byte++) {

            data_block[byte] = buff[byte + headers[i].start + HBLK_SIZE];
        }

        if (!headers[i].is_dir) {

            write_stream.write(data_block, headers[i].fsize);

        } else {

            strcat(dir, "\\");
            strcat(dir, headers[i].fname);

            CreateDirectory(dir, NULL);
        }

//        printf("\tdata_block_size: %d\n", data_block_size);

        write_stream.close();
    }
}
