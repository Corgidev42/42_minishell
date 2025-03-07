# 🛠 Compiler et flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline

# 🏆 Nom du projet
PROJECT_NAME = minishell

# 🖥️ Détection de l'OS
UNAME_S := $(shell uname -s)

# 🔧 Configuration spécifique à l'OS
ifeq ($(UNAME_S), Linux)
	CFLAGS   += -I/usr/include
	LDFLAGS  += -L/usr/lib
else ifeq ($(UNAME_S), Darwin)
	CFLAGS   += -I/opt/homebrew/include
	LDFLAGS  += -L/opt/homebrew/lib -framework Cocoa
else ifeq ($(UNAME_S), Windows_NT)
	CFLAGS   +=
	LDFLAGS  +=
endif

# 📂 Répertoires
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# 📌 Inclure les fichiers headers
INCLUDES = -I$(INCLUDE_DIR) $(LIBFT_INCLUDE)

# 🔍 Trouver tous les fichiers source (.c)
# SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
SRC_FILES =	$(SRC_DIR)/app.c \
			$(SRC_DIR)/built_in_bis.c \
			$(SRC_DIR)/built_in.c \
			$(SRC_DIR)/exec_ast.c \
			$(SRC_DIR)/exec_ast1.c \
			$(SRC_DIR)/exec_ast2.c \
			$(SRC_DIR)/input.c \
			$(SRC_DIR)/main.c \
			$(SRC_DIR)/minishell.c \
			$(SRC_DIR)/prepare_ast.c \
			$(SRC_DIR)/tokenizer.c \
			$(SRC_DIR)/tokenizer1.c \
			$(SRC_DIR)/utils.c \

# 🏗️ Création de la liste des fichiers objets correspondants
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# 📚 Gestion de la libft
LIBFT_DIR = mylib
LIBFT = $(LIBFT_DIR)/lib/libft.a
LIBFT_REPO = https://github.com/Corgidev42/mylib.git
LIBFT_INCLUDE = -I$(LIBFT_DIR)/include

# 🎯 Compilation principale
all: $(PROJECT_NAME)

# 🏗️ Création de l'exécutable
$(PROJECT_NAME): $(LIBFT) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OS_FLAGS) $(INCLUDES) $(OBJ_FILES) $(LDFLAGS) $(LIBFT) -o $@
	@echo "✅ Compilation terminée !"

# 🛠️ Compilation des fichiers objets (avec support des sous-dossiers)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)  # Création des sous-dossiers si nécessaire
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	@echo "🔨 Compilé : $< -> $@"

# 📂 Création du dossier obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# 📦 Clonage de libft si nécessaire (ne se lance que si le dossier n'existe pas)
$(LIBFT_DIR):
	git clone $(LIBFT_REPO) $(LIBFT_DIR)

# 📦 Compilation de libft (seulement si nécessaire)
$(LIBFT): | $(LIBFT_DIR)
	@if [ ! -f $(LIBFT) ]; then \
		echo "📥 Compilation de libft..."; \
		make -C $(LIBFT_DIR); \
	else \
		echo "✅ libft déjà compilée."; \
	fi

# 🧹 Nettoyage des fichiers objets
clean:
	rm -rf $(OBJ_DIR)
	if [ -d $(LIBFT_DIR) ]; then make -C $(LIBFT_DIR) clean; fi
	@echo "🗑️  Cleaned object files."

# 🗑️ Nettoyage complet (objets, exécutable ET libft)
fclean: clean
	rm -f $(PROJECT_NAME)
	rm -rf $(LIBFT_DIR)
	@echo "🗑️  Fully cleaned (executables, object files & libft directory)."

# 🔄 Reconstruire tout sans supprimer libft (évite de re-puller si la lib est à jour)
re: clean all

# 🔄 Suppression et recompilation forcée de libft
relibft:
	rm -rf $(LIBFT_DIR)
	$(MAKE) $(LIBFT)

# 🚀 Lancer le programme
run: all
	@clear
	@./$(PROJECT_NAME)

.PHONY: all clean fclean re relibft run
