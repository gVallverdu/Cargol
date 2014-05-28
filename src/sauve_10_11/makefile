
# -------------------------------------------------------------------
#
#   makefile 
#
# -------------------------------------------------------------------

# nom de l'executable :
EXECUTABLE = cargol

# choix du compilateur :
COMPILATEUR = gcc

# options compilations :
OPTIONS = -c -O3 -Wall -Wextra 

# liste de toutes les sources :
SRC = $(wildcard *.c)

# liste de tous les objets correspondant :
OBJ = $(patsubst %.c, %.o, $(SRC))

# commande par defaut
all: dev

# edition de liens de tous les objets (-lm pour la lib math.h)
install: $(OBJ) 
	$(COMPILATEUR) $(OBJ) -o $(EXECUTABLE) -lm 
	mv $(EXECUTABLE) ../
	mv *.o ../obj/

# dev
dev: $(OBJ) 
	$(COMPILATEUR) $(OBJ) -o $(EXECUTABLE) -lm 
	mv $(EXECUTABLE) ../$(EXECUTABLE)_dev
	mv *.o ../obj/

# on compile tous les fichiers sources :
%.o: %.c
	$(COMPILATEUR) $(OPTIONS) $*.c

# dependances :
include dependances

# pour effacer tous les objet :
.PHONY: clean
clean:
	rm -rf ../obj/*.o

