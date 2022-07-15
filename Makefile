DESTDIR=
PREFIX =/usr/local
HEADERS=io/fcopy.h io/freplace.h io/stdio-rec.h
all:
install:
	mkdir -p $(DESTDIR)$(PREFIX)/include/io
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/io

## -- manpages --
install: install-man
install-man: ./doc/fcopy.3.md 
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	cp ./doc/fcopy.3  $(DESTDIR)$(PREFIX)/share/man/man3
## -- manpages --
## -- license --
install: install-license
install-license: LICENSE
	mkdir -p $(DESTDIR)$(PREFIX)/share/doc/c-streams
	cp LICENSE $(DESTDIR)$(PREFIX)/share/doc/c-streams
## -- license --
