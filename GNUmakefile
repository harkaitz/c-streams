PROJECT  =c-streams
VERSION  =1.0.0
DESTDIR  =
PREFIX   =/usr/local
CC       =gcc -pedantic-errors -std=c99 -Wall
HEADERS  =io/freplace.h io/fcopy.h
PROGRAMS =tools/freplace$(EXE) tools/fcopy$(EXE)
##
all: $(PROGRAMS)
install: $(PROGRAMS)
	@mkdir -p $(DESTDIR)$(PREFIX)/include/io
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/io
	cp $(PROGRAMS) $(DESTDIR)$(PREFIX)/bin
clean:
	rm -f $(PROGRAMS)
tools/%$(EXE): tools/%.c $(HEADERS)
	$(CC) -o $@ $< $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) $(LIBS)
## -- BLOCK:license --
install: install-license
install-license: 
	@mkdir -p $(DESTDIR)$(PREFIX)/share/doc/$(PROJECT)
	cp LICENSE  $(DESTDIR)$(PREFIX)/share/doc/$(PROJECT)
## -- BLOCK:license --
## -- BLOCK:man --
install: install-man
install-man:
	@mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	cp ./.git/refs/tags/v0.1 $(DESTDIR)$(PREFIX)/share/man/man1
	@mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	cp ./doc/fcopy.3 $(DESTDIR)$(PREFIX)/share/man/man3
## -- BLOCK:man --
