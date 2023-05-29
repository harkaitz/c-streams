DESTDIR  =
PREFIX   =/usr/local
HEADERS  =io/freplace.h io/fcopy.h
CC       =cc
CFLAGS   =-Wall -g
PROGRAMS =tools/freplace tools/fcopy
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
## -- manpages --
install: install-man1
install-man1:
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	cp ./.git/refs/tags/v0.1 $(DESTDIR)$(PREFIX)/share/man/man1
install: install-man3
install-man3:
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	cp ./doc/fcopy.3 $(DESTDIR)$(PREFIX)/share/man/man3
## -- manpages --
## -- license --
install: install-license
install-license: LICENSE
	mkdir -p $(DESTDIR)$(PREFIX)/share/doc/c-streams
	cp LICENSE $(DESTDIR)$(PREFIX)/share/doc/c-streams
## -- license --
