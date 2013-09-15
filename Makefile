##
#	Makefile para construir el leector de archivos shape y generara el formato intermedio
#	
#	AAFR, GENEC, S.A. de C.V.
#	22 de noviembre de 2003
#

CC= gcc -c
CLINKER= gcc -o

CFLAGS= -O3 -Wall

LIBS= -lshp

OBJ= 	main.o \
	saca_objetos.o \
	saca_records.o \
	imprime_xml.o

OBJ2= dbfopen.o \
	shpopen.o
 
DIR_SRC=./src/
DIR_BIN=./bin/

all: clean $(OBJ)
	$(CLINKER) $(DIR_BIN)shp2int.exe $(OBJ) $(LIBS) $(CFLAGS)

all2: clean $(OBJ) $(OBJ2)
	$(CLINKER) $(DIR_BIN)shp2int.exe $(OBJ) $(OBJ2) $(CFLAGS)

main.o:
	$(CC) $(DIR_SRC)main.c $(CFLAGS)

saca_objetos.o:
	$(CC) $(DIR_SRC)saca_objetos.c $(CFLAGS)

saca_records.o:
	$(CC) $(DIR_SRC)saca_records.c $(CFLAGS)

imprime_xml.o:
	$(CC) $(DIR_SRC)imprime_xml.c $(CFLAGS)


shpopen.o:
	$(CC) $(DIR_SRC)shpopen.c $(CFLAGS)

dbfopen.o:
	$(CC) $(DIR_SRC)dbfopen.c $(CFLAGS)

install:
	
	@cp $(DIR_BIN)*.exe $(DIR_BIN)*.sh ~/bin/ -v

uninstall:
	@rm ~/bin/shp2int.exe ~/bin/shp2int.sh -v

docs: borradocs
	doxygen shp2int.dox 


borradocs:

	@rm -rf docs
	@mkdir docs

clean:
#	@elim
	@rm -rfv *.o

sc: clean
	@rm -rf *.exe
