#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "displayer.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <class file>\n", argv[0]);
        return 1;
    }

    struct ClassFile *cf = readFile(argv[1]);
    if (cf == NULL) {
        fprintf(stderr, "Erro ao ler o arquivo de classe.\n");
        return 1;
    }
    printf("Arquivo de classe lido com sucesso: %s\n\n", argv[1]);
    printf("---- Início do Display ----\n");
    displayClassFile(cf);
    freeClassFile(cf);

    return 0;
}
