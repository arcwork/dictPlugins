CC=gcc
CFLAGS=-I. -Wall -std=gnu99
LIBS=-ldl

all: Dict

Dict: DictDaemon.o PluginManager.o
	$(CC) -o Dict DictDaemon.o PluginManager.o $(CFLAGS) $(LIBS)

install: Dict
	mkdir -p ../APP
	cp Dict ../APP/Dict

clean:
	-rm *.o Dict
	$(RM) ../APP/Dict
	$(RM) Dict.o

