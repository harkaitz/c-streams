PROJECT  =c-streams
VERSION  =1.0.0
DESTDIR  =
PREFIX   =/usr/local
CC       =cc
CFLAGS   =-Wall -g

HEADERS  =io/freplace.h io/fcopy.h
PROGRAMS =tools/freplace tools/fcopy

all:
install:

## -- Generic targets
all: $(PROGRAMS)
install: $(PROGRAMS)
	mkdir -p $(DESTDIR)$(PREFIX)/include/io
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/io
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(PROGRAMS) $(DESTDIR)$(PREFIX)/bin
clean:
	rm -f $(PROGRAMS)
## -- tools
tools/%: tools/%.c $(HEADERS)
	$(CC) -o $@ $< $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) $(LIBS)
## -- BLOCK:license --
install: install-license
install-license: 
	mkdir -p $(DESTDIR)$(PREFIX)/share/doc/$(PROJECT)
	cp LICENSE README.md $(DESTDIR)$(PREFIX)/share/doc/$(PROJECT)
update: update-license
update-license:
	ssnip README.md
## -- BLOCK:license --
## -- BLOCK:man --
update: update-man
update-man:
	make-h-man update
install: install-man
install-man:
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	cp ./.git/refs/tags/v0.1 $(DESTDIR)$(PREFIX)/share/man/man1
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	cp ./doc/fcopy.3 $(DESTDIR)$(PREFIX)/share/man/man3
## -- BLOCK:man --
