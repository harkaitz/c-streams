DESTDIR  =
PREFIX   =/usr/local
HEADERS  =io/fcopy.h io/freplace.h io/stdio-rec.h io/incbin-fp.h
CC       =gcc
CFLAGS   =-Wall -g
PROGRAMS =tools/freplace tools/fcopy


## ---------------------------------------------------
all: $(PROGRAMS)
install: $(PROGRAMS)
	@echo 'I include/ $(HEADERS)'
	@mkdir -p $(DESTDIR)$(PREFIX)/include/io
	@cp $(HEADERS) $(DESTDIR)$(PREFIX)/include/io
	@echo 'I bin/ $(PROGRAMS)'
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp $(PROGRAMS) $(DESTDIR)$(PREFIX)/bin
clean:
	@echo 'D $(PROGRAMS)'
	@rm -f $(PROGRAMS)

## ---------------------------------------------------
tools/%: tools/%.c $(HEADERS)
	@echo 'B $@ $<'
	@$(CC) -o $@ $< $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) $(LIBS)



## -- manpages --
install: install-man1
install-man1:
	@echo 'I share/man/man1/'
	@mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	@echo 'I share/man/man1/v0.1'
	@cp ./.git/refs/tags/v0.1 $(DESTDIR)$(PREFIX)/share/man/man1
install: install-man3
install-man3:
	@echo 'I share/man/man3/'
	@mkdir -p $(DESTDIR)$(PREFIX)/share/man/man3
	@echo 'I share/man/man3/fcopy.3'
	@cp ./doc/fcopy.3 $(DESTDIR)$(PREFIX)/share/man/man3
## -- manpages --
## -- license --
install: install-license
install-license: LICENSE
	@echo 'I share/doc/c-streams/LICENSE'
	@mkdir -p $(DESTDIR)$(PREFIX)/share/doc/c-streams
	@cp LICENSE $(DESTDIR)$(PREFIX)/share/doc/c-streams
## -- license --
