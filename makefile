# --- Variáveis de Compilação ---

# Compilador
CC = gcc

# --- Diretórios ---
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# --- Flags de Compilação ---
# Padrão C99, warnings, debug, e o caminho dos headers (-I)
CFLAGS = -std=c99 -Wall -Wextra -g -I$(INC_DIR)

# Flags para o AddressSanitizer (ASAN)
CFLAGS_ASAN = $(CFLAGS) -fsanitize=address

# Bibliotecas (ex: -lm para a biblioteca de matemática)
LIBS = -lm

# --- Gestão de Ficheiros ---

# Arquivos fonte .c (nomes base
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Mapeia os .c para os caminhos completos dos ficheiros objeto em build/
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))


# --- Nomes dos Alvos (Executáveis) ---
TARGET_NAME = class-reader
TARGET = $(BIN_DIR)/$(TARGET_NAME)

ASAN_TARGET = $(BIN_DIR)/$(TARGET_NAME)-asan


# Ferramentas de limpeza (RM = remove)
RM = rm -f


# --- Deteção de OS (para .exe e 'del') ---
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    ASAN_TARGET := $(ASAN_TARGET).exe
    RM = del /Q /F
endif


# --- Regras (Targets) ---

# Regra padrão (o que acontece quando você digita 'make' ou 'make all')
all: $(TARGET)

# Regra para linkar o executável final
# Depende que o diretório $(BIN_DIR) e todos os objetos $(OBJS) existam
$(TARGET): $(BIN_DIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	@echo "Executável '$(TARGET)' criado com sucesso."

# Regra para compilar cada .o a partir de um .c e colocá-lo no BUILD_DIR
# Ex: compila "src/main.c" para "build/main.o"
# A dependência "| $(BUILD_DIR)" garante que o diretório existe antes de compilar
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação dos diretórios bin e build se não existirem
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@


# --- Regras de Limpeza e Análise ---

# Remove os ficheiros compilados de 'bin' e 'build'
clean:
	$(RM) $(TARGET)
	$(RM) $(ASAN_TARGET)
ifeq ($(OS),Windows_NT)
	$(RM) $(BUILD_DIR)\\*.o
	@echo "Arquivos compilados removidos de $(BIN_DIR) e $(BUILD_DIR)."
else
	$(RM) $(BUILD_DIR)/*.o
	@echo "Arquivos compilados removidos de $(BIN_DIR) e $(BUILD_DIR)."
endif


# Compila uma versão com AddressSanitizer
asan: $(BIN_DIR) $(BUILD_DIR)
	$(CC) $(CFLAGS_ASAN) -o $(ASAN_TARGET) $(SRCS) $(LIBS)
	@echo "Compilado com AddressSanitizer. Execute: ./$(ASAN_TARGET) <arquivo.class>"


# Executa o analisador estático cppcheck
check:
	@echo "Executando cppcheck em todo o projeto..."
	cppcheck --enable=all --suppress=missingIncludeSystem --check-level=exhaustive --error-exitcode=1 -I$(INC_DIR) $(SRCS)

# Define regras que não geram arquivos
.PHONY: all clean asan check