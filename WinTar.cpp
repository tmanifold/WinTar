

#include <cstdlib>
#include <cstring>
#include "TarFile.h"

using namespace std;

int main(int argc, char *argv[]) {

    TarFile *tarf;

    if (argc <= 2) {

        printf("invalid args\n");

    } else  {

        if (argc == 3) {

            tarf = new TarFile(argv[2]);

            if (strcmp(argv[1], "-l") == 0){

                tarf->list_tar();

            } else if (strcmp(argv[1], "-x") == 0) {

                tarf->untar();
            }
        } else if (argc >= 4) {

            std::vector<char *> file_list;

            if (strcmp(argv[1], "-c") == 0) {

                printf("file_name: %s\n", argv[2]);

                for (int i = 3; i < argc; i++) {

                    file_list.push_back(argv[i]);

                    printf("added %s\n", file_list[i-3]);
                }
            }
            tarf = new TarFile(file_list);
        }
    }
    return 0;
}
