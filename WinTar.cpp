
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "TarFile.h"


using namespace std;

int main(int argc, char *argv[]) {

    if (argc == 1) {
        printf("invalid args\n");
    } else if (strcmp(argv[1], "-l") == 0){
        //printf("processing %s\n", argv[2]);
        TarFile tarf(argv[2]);
        tarf.list_tar();
    } else if (strcmp(argv[1], "-x") == 0) {
        TarFile tarf(argv[2]);
        tarf.untar();
    }
    return 0;
}
