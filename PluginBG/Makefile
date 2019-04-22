CFLAGS := -fPIC -g -Wall -Werror
CC := gcc
MAJOR := 0
MINOR := 1
NAME := BG
VERSION := $(MAJOR).$(MINOR)

lib: libplugin$(NAME).so.$(VERSION)

libplugin$(NAME).so.$(VERSION): plugin$(NAME).o
	$(CC) -shared -Wl,-soname,libplugin$(NAME).so.$(MAJOR) $^ -o $@

install: lib
	mkdir -p ../APP
	mkdir -p ../APP/plugins
	cp libplugin$(NAME).so.$(VERSION) ../APP/plugins

clean:
	$(RM) *.o *.so*
	$(RM) ../APP/plugins/libplugin$(NAME).so.$(VERSION)
