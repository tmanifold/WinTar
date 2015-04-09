
#include "TarFile.h"
#include <cstdlib>
#include <cstring>

TarFile::TarFile(char *n) {
    this->file_stream.open(n, ios::in);
    if (file_stream.is_open()) {
        this->fname = n;

        file_stream.seekg(0, ios::end);

        this->buff_size = file_stream.tellg();

        file_stream.seekg(0, ios::beg);

        this->buff = (char *) malloc(buff_size);

        printf("\n%s: %d bytes\n\n", this->fname, this->buff_size);
        //printf("buffer: %d bytes\n", buff_size);
    } else {
        printf("Could not open files: %s\n", n);

    }
}

TarFile::~TarFile() {
    if (file_stream.is_open()) {
        this->file_stream.close();
    }
}

void TarFile::read_head() {
    file_stream.read(buff, buff_size);

    int pos = 0;
    int s = 0;
    char name[100];
    char b[512];
    int byte_count = 0;

    //printf("pos\t\tfname\t\tfsize\t\tbyte_count\n");
    printf("name\t\tsize (bytes)\n");

    while (byte_count < buff_size) {
        _head = TarHeader();
       // printf("\npos: %d\n", pos);

        for (int i = 0; i < 512; i++) {
            b[i] = buff[i + pos];
        }
        for (int i = 0; i < 100; i++) {
            name[i] = b[i];
            _head.fname[i] = b[i];
        }

        s = otoi(&b[124], 11);

        _head.fsize = s;
        _head.start = pos;
        headers.push_back(_head);

        printf("%s\t\t%d\n", name, s);
        //printf("%d\t\t%s\t\t%d bytes\t%d\n", pos, name, s, byte_count);
        //printf("%d\t\t%s\t\t%d bytes\t%d\n", pos, h.fname, h.fsize, byte_count);

        pos += round512(s) + 512;
        byte_count += 512 + round512(s) * 3;
    }
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

    if (n <= 512) {
        rounded = 512;
    } else {
        rounded = (n + 512) - (n % 512);
    }
    return rounded;
}

void TarFile::untar() {
    read_head();
    for (int i = 0; i < headers.size(); i++) {

        long data_block_size = round512(headers[i].fsize);
        char *data_block = (char *) malloc(data_block_size);

        this->write_stream.open(headers[i].fname, ios::out);
        printf("opened %s\n", headers[i].fname);

        for (int byte = 0; byte < headers[i].fsize; byte++) {
            data_block[i] = buff[i + headers[i].start + 513];
        }
        write_stream.write(data_block, headers[i].fsize);
        printf("\tdata_block_size: %d\n", data_block_size);

        write_stream.close();

    }
}
