# -------------------------
# Compilador e Flags
# -------------------------
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude -DTAM_Y=$(ARG)

# -------------------------
# Diretórios
# -------------------------
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# -------------------------
# Arquivos fonte e objetos
# -------------------------
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(wildcard include/*.hpp)

# -------------------------
# Nome do executável
# -------------------------
TARGET := $(BIN_DIR)/tp2.out

# -------------------------
# Regra padrão (default)
# -------------------------
all: $(TARGET)

# -------------------------
# Linkagem do executável
# -------------------------
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# -------------------------
# Compilação dos objetos
# -------------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# -------------------------
# Criação de diretórios (se não existirem)
# -------------------------
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# -------------------------
# Limpeza dos arquivos gerados
# -------------------------
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# -------------------------
# Definição de regras "falsas"
# -------------------------
.PHONY: all clean
