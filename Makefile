DESTDIR=
PREFIX =/usr/local
HEADERS=io/fcopy.h io/freplace.h io/stdio-rec.h

## ---------------------------------------------------
all:
install:
	mkdir -p $(DESTDIR)$(PREFIX)/include/io
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/io

## -- manpages --
ifneq ($(PREFIX),)
MAN_3=./doc/fcopy.3 
install: install-man3
install-man3: $(MAN_3)
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	cp $(MAN_3) $(DESTDIR)$(PREFIX)/share/man/man3
endif
## -- manpages --
## -- license --
ifneq ($(PREFIX),)
install: install-license
install-license: LICENSE
	mkdir -p $(DESTDIR)$(PREFIX)/share/doc/c-streams
	cp LICENSE $(DESTDIR)$(PREFIX)/share/doc/c-streams
endif
## -- license --
