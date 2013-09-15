##
#	Makefile para construir el leector de archivos shape y generara el formato intermedio
#	
#	AAFR <alffore@gmail.com>
#	2003-2013
#

CC= gcc -c
CLINKER= gcc -o

CFLAGS= -O3 -Wall

LIBS= -lshp -lm

OBJ= main.o \
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

%.o: $(DIR_SRC)%.c
	$(CC) $(CFLAGS)  $<

install:
	
	@cp $(DIR_BIN)*.exe $(DIR_BIN)*.sh ~/bin/ -v

uninstall:
	@rm ~/bin/shp2int.exe ~/bin/shp2int.sh -v

docs: borradocs
	doxygen docs/shp2int.dox 


borradocs:

	@rm -rf docs/html
	

clean:
	@rm -rfv *.o

sc: clean
	@rm -rf *.exe
