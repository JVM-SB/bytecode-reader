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
        fprintf(stderr, "Uso: %s <arquivo.class> [Modo] [Metodo]\n", argv[0]);
        fprintf(stderr, "Modos:\n");
        fprintf(stderr, "  -d    : Modo Exibidor\n");
        fprintf(stderr, "  -r    : Modo JVM [Padrao]\n");
        fprintf(stderr, "Metodo: Nome do metodo de entrada (padrao: main)\n");
        return 1;
    }

    const char *fileName = argv[1];
    
    // Verifica o modo (-d ou -r), padrão é -r
    const char *mode = (argc > 2) ? argv[2] : "-r";
    
    // Verifica o método de entrada, padrão é "main"
    const char *entryMethodName = (argc > 3) ? argv[3] : "main";

    struct ClassFile *cf = readFile(fileName);
    if (cf == NULL) {
        fprintf(stderr, "Erro: Nao foi possivel ler o arquivo .class.\n");
        exit(-1);
    }
    
    if (strcmp(mode, "-d") == 0) {
        displayClassFile(cf);
    } else {
        JVM *jvm = calloc(1, sizeof(JVM));
        if (jvm == NULL) {
            fprintf(stderr, "Erro: Nao foi possivel alocar a JVM.\n");
            exit(-1);
        }

        initJVM(jvm);
        initExecutionEngine();

        loadClass(jvm, cf);

        // Busca dinâmica pelo método de entrada
        method_info *entryMethod = findMethod(cf, entryMethodName);
        if (entryMethod == NULL) {
            fprintf(stderr, "Erro: Metodo '%s' nao foi encontrado na classe.\n", entryMethodName);

            freeClassFile(cf);
            freeJVM(jvm);
            exit(-1);
        }

        code_attribute *code = findCodeAttribute(cf, entryMethod);
        if(code == NULL) {
            fprintf(stderr, "Erro: Metodo '%s' nao possui atributo Code (pode ser nativo ou abstrato).\n", entryMethodName);

            freeClassFile(cf);
            freeJVM(jvm);
            exit(-1);
        }

        Frame *initialFrame = createFrame(jvm, cf, code, NULL);
        jvm->current_frame = initialFrame;

        // Nota: Se o método exigir parâmetros (ex: somar(II)I), as variáveis locais
        // iniciam zeradas. Para testes reais, futuramente precisaremos de um parser de argumentos.
        
        execute(jvm);
        
        // Se o método retornar valor (ireturn/lreturn...), ele estará no topo da pilha do caller.
        // Como este é o frame inicial, o valor é descartado ao deletar o frame em ireturn_impl,
        // mas a execução terá ocorrido com sucesso.

        freeJVM(jvm);
    }

    freeClassFile(cf);

    return 0;
}