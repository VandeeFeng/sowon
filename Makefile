UNAMEOS = $(shell uname)

COMMON_CFLAGS=		-Wall -Wextra -std=c99 -pedantic
SDL2_CFLAGS+=		`pkg-config --cflags sdl2` $(COMMON_CFLAGS)
RGFW_CFLAGS+=		$(COMMON_CFLAGS)
COMMON_LIBS=		-lm
SDL2_LIBS=			`pkg-config --libs sdl2` $(COMMON_LIBS)
ifeq ($(UNAMEOS),Darwin)
RGFW_LIBS=			$(COMMON_LIBS) -framework CoreVideo -framework Cocoa -framework OpenGL -framework IOKit
else
RGFW_LIBS=			-lX11 -lXrandr -lGLX -lGL $(COMMON_LIBS)
endif
PREFIX?=		/usr/local
INSTALL?=		install

.PHONY: all
all: Makefile sowon sowon_rgfw man

sowon_rgfw: main_rgfw.c digits.h penger_walk_sheet.h
	$(CC) $(RGFW_CFLAGS) -o sowon_rgfw main_rgfw.c $(RGFW_LIBS)

sowon: main.c digits.h penger_walk_sheet.h
	$(CC) $(SDL2_CFLAGS) -o sowon main.c $(SDL2_LIBS)

digits.h: png2c digits.png
	./png2c digits.png digits > digits.h

penger_walk_sheet.h: png2c penger_walk_sheet.png
	./png2c penger_walk_sheet.png penger > penger_walk_sheet.h

png2c: png2c.c
	$(CC) $(COMMON_CFLAGS) -o png2c png2c.c -lm

docs/sowon.6.gz: docs/sowon.6
	gzip -c docs/sowon.6 > docs/sowon.6.gz

.PHONY: man
man: docs/sowon.6.gz

.PHONY: clean
clean:
	rm sowon docs/sowon.6.gz png2c

.PHONY: install
install: all
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -C ./sowon $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -C ./sowon_rgfw $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/man/man6
	$(INSTALL) -C docs/sowon.6.gz $(DESTDIR)$(PREFIX)/man/man6
