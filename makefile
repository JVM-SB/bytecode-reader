# --- Variáveis de Compilação ---

# Compilador
CC = gcc

# Diretórios
SRC_DIR = src
INC_DIR = src

# Flags do C: Padrão C99, warnings, debug, e o caminho dos headers (-I)
CFLAGS = -std=c99 -Wall -Wextra -g -I$(INC_DIR)

# Flags para o AddressSanitizer (ASAN)
CFLAGS_ASAN = $(CFLAGS) -fsanitize=address

# Arquivos fonte .c (apenas os nomes base)
SRCS = main.c reader.c instructions.c displayer.c

# Arquivos objeto .o (serão criados na raiz)
OBJS = $(SRCS:.c=.o)


# --- Nomes dos Alvos (Executáveis) ---

# Nome do executável principal
TARGET = class-reader

# Ferramentas de limpeza (RM = remove)
RM = rm -f

# --- Detecção de OS ---
ifeq ($(OS),Windows_NT)
	TARGET := $(TARGET).exe
	RM = del /Q /F
else
	LIBS = -lm
endif


# --- Regras (Targets) ---

# Regra padrão (o que acontece quando você digita 'make' ou 'make all')
all: $(TARGET)

# Regra para linkar o executável final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	@echo "Executável '$(TARGET)' criado na raiz do projeto."

# Regra para compilar cada .o a partir de um .c
# $< é o arquivo .c
# $@ é o arquivo .o
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove os arquivos compilados da raiz
clean:
	$(RM) $(TARGET) $(OBJS)
	$(RM) $(TARGET)_asan.exe $(TARGET)_asan
	@echo "Arquivos compilados removidos."


# --- Regras de Análise (Bônus) ---

# Compila uma versão com AddressSanitizer
asan:
	# $(SRCS:%=$(SRC_DIR)/%) expande para "src/main.c src/reader.c ..."
	$(CC) $(CFLAGS_ASAN) -o $(TARGET)_asan $(SRCS:%=$(SRC_DIR)/%) $(LIBS)
	@echo "Compilado com AddressSanitizer. Execute: ./$(TARGET)_asan <arquivo.class>"

# Executa o analisador estático cppcheck no diretório src
check:
	@echo "Executando cppcheck em todo o projeto..."
	cppcheck --enable=all --suppress=missingIncludeSystem --check-level=exhaustive --error-exitcode=1 -I$(INC_DIR) $(SRCS:%=$(SRC_DIR)/%)

# Define regras que não geram arquivos
.PHONY: all clean asan check
