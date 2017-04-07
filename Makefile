#$* é o %
#$@ é o nome da regra
#$< é o nome da primeira dependencia
#$^ é o nome de todas as dependencias
#
#---------------------------------------------------------------------------------------------
#defaul/linux
#---------------------------------------------------------------------------------------------
#
#comandos
CC = g++
RMDIR = rm -rf
RM = rm -f
CLEAR = clear
#caminhos
HEADER_PATH = include
SOURCE_PATH = src
BIN_PATH = bin
DEP_PATH = dep
#arquivo
EXEC = JOGO
#arquivos
CPP_FILES = $(wildcard $(SOURCE_PATH)/*.cpp)
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))
DEP_FILES = $(wildcard $(DEP_PATH)/*.d)
#flags
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.d
DIRECTIVES = -std=c++11 -Wall -Wextra -c -I $(HEADER_PATH) -Wno-unused-result -Wno-unused-parameter
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
# #
# #---------------------------------------------------------------------------------------------
# #windows
# #---------------------------------------------------------------------------------------------
# #
# ifeq($(OS),Windows_NT)
# #comandos
# RMDIR = rd /s /q
# RM = del
# CLEAR = cls
# #caminhos
# SDL_PATH = C:SDL2-2.0.5\x86_64-w64-mingw32
# #arquivo
# EXEC := $(EXEC).exe
# #flags
# DIRECTIVES += -I $(SDL_PATH)\include
# LIBS = -L $(SDL_PATH)\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
# #
# #---------------------------------------------------------------------------------------------
# #mac os
# #---------------------------------------------------------------------------------------------
# #
# else
# UNAME_S := $(shell uname -s)
# ifeq($(UNAME_S), Darwin)
# #flags
# LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf
# endif
# endif




#---------------------------------------------------------------------------------------------
#regras
#---------------------------------------------------------------------------------------------



#regra default
all : $(BIN_PATH).folder $(DEP_PATH).folder $(EXEC)

#roda o jogo
run : all
	$(CLEAR) && ./$(EXEC) && $(CLEAR)

#compila JOGO, linkando todos os .o
$(EXEC) : $(OBJ_FILES)
	$(CC) -o $@ $^ $(LIBS)
	#g++  -o JOGO X.o Y.o ... -lSDL2 ...


#compila os .cpp criando os .o
$(BIN_PATH)/%.o : $(SOURCE_PATH)/%.cpp
	$(CC) $(DEP_FLAGS) -c -o $@ $< $(DIRECTIVES)

-include $(DEP_FILES)

#cria os diretorios caso nao existam
# ifeq($(OS),Windows_NT)
# 	@if not exist $(DEP_PATH) @mkdir $(DEP_PATH)
# 	@if not exist $(BIN_PATH) @mkdir $(BIN_PATH)
# else
	# @mkdir -p $(DEP_PATH) $(BIN_PATH)
# endif
$(BIN_PATH).folder:
	@mkdir -p $(BIN_PATH)

$(DEP_PATH).folder:
	@mkdir -p $(DEP_PATH)


clean :
	$(RMDIR) $(BIN_PATH) $(DEP_PATH)
	$(RM) $(EXEC)
#
#---------------------------------------------------------------------------------------------
#extras
#---------------------------------------------------------------------------------------------
#
print-% : ; @echo $* = \'$($*)\'

debug : DIRECTIVES += -ggdb -O0
debug : all

release : DIRECTIVES += -Ofast -mtune=native
release : all




make_new_file: make_class/make_new_file.cpp
	$(CC) -o make_class/make_new_file make_class/make_new_file.cpp -std=c++11

make_class-%-hpp: make_new_file
	./make_class/make_new_file $* a && subl include/$*.hpp && subl src/$*.cpp

make_class-%-cpp: make_new_file
	./make_class/make_new_file $* a a && subl include/$*.hpp && subl src/$*.cpp

make_class-%: make_new_file
	./make_class/make_new_file $* && subl include/$*.hpp && subl src/$*.cpp
