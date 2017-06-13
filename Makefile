
# Assume-se o Linux como sistema operacional padrão:
# -----------------------------------------------------------------------------------------------------------------------------------------------------------
# Compilador a ser utilizado
CC=g++
# Comando para remover um diretório recursivamente e ignorar caso não exista
RMDIR = rm -rf
# Comando para remover um arquivo e ignorar caso não exista
RM = rm -f

# "Flags" para a geração automática das dependências
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.d
# Diretivas que são utilizadas na compilação de cada objeto
DIRECTIVES = -std=c++11 -Wall -Wextra -c -I $(HEADER_PATH)
# Diretivas que são utilizadas na "linkagem" dos objetos gerando o executável
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

# Um caminho para guardar e acessar cada tipo de arquivo .h (headers),
# .cpp (sources), .o (objects) e .d (dependencies), respectivamente
HEADER_PATH = include
SRC_PATH = src
BIN_PATH = bin
DEP_PATH = dep

# Uma lista de arquivos para cada tipo de arquivo:
# .cpp .o e .d respectivamente
CPP_FILES = $(wildcard $(SRC_PATH)/*.cpp)
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))
DEP_FILES = $(wildcard $(DEP_PATH)/*.d)

EXEC = JOGO

# Caso o Sistema Operacional seja Windows, modifica-se as seguintes variáveis:
# -----------------------------------------------------------------------------------------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
# Comando para remover um diretório recursivamente e ignorar caso não exista
RMDIR = rd /s /q
#Comando para deletar um arquivo
RM = del
# Caminho para os "headers" da biblioteca SDL2 no Windows
SDL_PATH = C:\TDM-GCC-64

# Adiciona-se diretivas específicas do Windows
DIRECTIVES += -I $(SDL_PATH)\include\SDL2
# Substitui as diretivas de "linkagem" adicionando flags próprias do Windows
LIBS = -L $(SDL_PATH)\lib -lmingw32 -lSDL2main\
	   -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

# Nome do executável final (deve ter extensão .exe necessária para o Windows)
EXEC := $(EXEC).exe

else
UNAME_S := $(shell uname -s)

# Caso o Sistema Operacional seja Mac OS, modifica-se as seguintes variáveis

ifeq ($(UNAME_S), Darwin)
LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer\
	   -framework SDL2_ttf
endif
endif

# Regra default:
all: $(EXEC)
# Regra de criação do executável final:
$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LIBS)

#chama all e depois roda o jogo
run: clear all
ifeq ($(OS),Windows_NT)
	$(EXEC)
else
	./$(EXEC)
endif

# Regra de inferência para criação dos objetos de compilação: 
$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
# De acordo com o SO, tenta criar os diretórios
# dep e bin e ignora caso eles já existam
#----------------------------------------------------------------------------------------------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
	@if not exist $(DEP_PATH) @mkdir $(DEP_PATH)
	@if not exist $(BIN_PATH) @mkdir $(BIN_PATH)
else
	@mkdir -p $(DEP_PATH) $(BIN_PATH)
endif
#----------------------------------------------------------------------------------------------------------------------------------------------------------------
# Gera os objetos usando diretivas de compilação e as respectivas dependências 
	$(CC) $(DEP_FLAGS) -c -o $@ $< $(DIRECTIVES)
# Inclui as regras geradas pelos arquivos de dependência
-include $(DEP_FILES)

# Regra para limpar/deletar todos os arquivos e diretórios criados pelo make
clean:
	$(RMDIR) $(BIN_PATH) $(DEP_PATH)
	$(RM) $(EXEC)	

# Regra que estabelece que arquivos .d são "preciosos"
.PRECIOUS: $(DEP_PATH)/%.d
# Regra que torna as dependências "phony"
.PHONY: debug clean release

# Regra para printar uma variável no terminal. Auxilia no debug do Makefile
print-% : ; @echo $* = $($*)

# Regra que inclui diretivas de debug na compilação
debug: DIRECTIVES += -ggdb -O0
debug: all

# Regra que inclui diretivas de compilação otimizada
release: DIRECTIVES += -Ofast -mtune=native
release: all

clear:
ifeq ($(OS),Windows_NT)
	cls && cls
else
	clear && clear
endif
remake: clear clean all

make_new_file: make_class/make_new_file.cpp
	$(CC) -o make_class/make_new_file make_class/make_new_file.cpp -std=c++11

make_class-%-hpp: make_new_file
	./make_class/make_new_file $* a && xdg-open include/$*.hpp && xdg-open src/$*.cpp

make_class-%-cpp: make_new_file
	./make_class/make_new_file $* a a && xdg-open include/$*.hpp && xdg-open src/$*.cpp

make_class-%: make_new_file
	./make_class/make_new_file $* && xdg-open include/$*.hpp && xdg-open src/$*.cpp

make_component-%: make_new_file
	./make_class/make_new_file $* a a a && xdg-open include/component$*.hpp && xdg-open src/component$*.cpp
