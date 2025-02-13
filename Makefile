#CC : le compilateur à utiliser
CC=gcc

#CFLAGS : les options de compilation (en C, voir les warning,...)
CFLAGS=-Wall

# les fichiers sources : tous les fichiers présents dans src/
SRC=$(wildcard src/*.c)

# les fichiers objets (.o)
OBJ=$(patsubst src/%.c,obj/%.o,$(SRC))

.PHONY : doc
# génération de l'executable
all: bin/exe


# installe l'executable dans le repertoire parent
# À noter que $< implique que l'on récupère le résultat de bin/exe placé après install
install: bin/exe
	cp $< exe 

# déinstalle l'executable du repertoire parent
uninstall:
	rm exe

doc:
	doxygen

#edition des liens : génération de l'exécutable à partir des .o 
bin/exe: $(OBJ)
	mkdir -p bin
	$(CC) $(OBJ) -o $@

# génération des .o à partir des .c et .h correspondants : 
obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

# Cible pour générer la documentation Doxygen
doc/html:
	@if [ ! -e Doxyfile ]; then doxygen -g; fi
	doxygen

#nettoyage : destruction des .o et de l'exécutable
clean:
	rm -rf obj bin doc/html