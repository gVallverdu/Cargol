# -------------------------------------------------------------------
#
#   makefile de cargol
#
# -------------------------------------------------------------------


# -------------------------------------------------------------------
#  options modifiables
# -------------------------------------------------------------------

# Nom de l'executable :
EXECUTABLE = cargol

# Choix du compilateur :
COMPILATEUR = gcc

# Options de compilations :
COPTIONS = -O3 -Wall -Wextra 
LOPTIONS = -lm 

# -------------------------------------------------------------------
#  Emplacement des sources et des objets
# -------------------------------------------------------------------

# Dossier racine du code :
CODE := $(shell pwd)

# Adresse des sources, des objets et des includes :
INCDIR = $(CODE)/include
SRCDIR = $(CODE)/src
OBJDIR = $(CODE)/obj
EXISTE := $(wildcard $(OBJDIR))

# Liste des fichiers sources :
SRC = $(wildcard $(SRCDIR)/*.c)
#SRC = $(foreach nom, $(SRCDIR), $(wildcard $(nom)/*.c))

# Liste des fichiers objets :
NOM = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(NOM)))

# -------------------------------------------------------------------
#  regle par defaut = aide
# -------------------------------------------------------------------

all: help

# -------------------------------------------------------------------
#  compilation
# -------------------------------------------------------------------

# regle edition de liens
install: dir $(OBJ) 
	$(COMPILATEUR) -o $(EXECUTABLE) $(OBJ) $(LOPTIONS)

# regle de compilation des sources
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILATEUR) -c $(COPTIONS) -I$(INCDIR) -o $@ $<

# -------------------------------------------------------------------
#  regles suplementaires
# -------------------------------------------------------------------

.PHONY: clean help dir purge

# creation du dossier obj si besoin :
ifeq ($(strip $(EXISTE)), )
dir: 
	mkdir $(OBJDIR)
else
dir: 
endif 

# pour effacer tous les objet :
clean:
	@rm -rv $(OBJDIR)

# pour effacer tous les objet et les executables :
purge:
	@rm -rv $(OBJDIR) $(EXECUTABLE)

# aide :
help:
	@echo
	@echo " Programme Cargol"
	@echo
	@echo "--------------------------------------------------------------------------"
	@echo " Options disponibles"
	@echo
	@echo " install : compilation et creation de l'executable cargol"
	@echo " clean   : effacement des fichiers objets"
	@echo " purge   : effacement des fichiers objets et des executables"
	@echo "--------------------------------------------------------------------------"
	@echo

# $@ = nom de la cible
# $< = nom de la premiere dependance

