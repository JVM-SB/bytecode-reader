#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "displayer.h"
#include "reader.h"
#include "memory_manager.h"
#include "execution_engine.h"
#include "frame.h"
#include "instructions.h"
#include "helpers.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.class>\n", argv[0]);
        fprintf(stderr, "Flags:\n");
        fprintf(stderr, "  -d    : Modo Exibidor\n");
        fprintf(stderr, "  -r    : Modo JVM [Padrao]\n");

        return 1;
    }

    const char *fileName = argv[1];
    const char *mode = (argc > 2) ? argv[2] : "-r";

    struct ClassFile *cf = readFile(fileName);
    if (cf == NULL) {
        fprintf(stderr, "Erro: Não foi possível ler o arquivo .class.\n");
        exit(-1);
    }
    
    if (strcmp(mode, "-d") == 0) {
        displayClassFile(cf);
        
    } else {
        JVM *jvm = calloc(1, sizeof(JVM));
        if (jvm == NULL) {
            fprintf(stderr, "Erro: Não foi possível alocar a JVM.\n");
            exit(-1);
        }

        initJVM(jvm);
        initExecutionEngine();

        loadClass(jvm, cf);

        method_info *mainMethod = findMethod(cf, "main");
        if (mainMethod == NULL) {
            fprintf(stderr, "Erro: Método 'main' não foi encontrado.\n");

            freeClassFile(cf);
            freeJVM(jvm);

            exit(-1);
        }

        code_attribute *code = findCodeAttribute(cf, mainMethod);
        if(code == NULL) {
            fprintf(stderr, "Erro: Metodo 'main' não possui atributo code.\n");

            freeClassFile(cf);
            free(jvm);

            exit(-1);
        }

        Frame *initialFrame = createFrame(jvm, cf, code, NULL);
        jvm->current_frame = initialFrame;

        execute(jvm);
        freeJVM(jvm);
    }

    freeClassFile(cf);

    return 0;
}
