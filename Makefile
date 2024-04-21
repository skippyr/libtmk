VERSION:=v10.0.0
CC:=cc
CFLAGS:=-std=c99 -Wpedantic -Wall -Wextra -Wno-unused-result -Os
SHELL:=bash
CPATH:=~/.local/share/include
LIBPATH:=~/.local/share/lib
MANPATH:=~/.local/share/man

.PHONY: all clean install uninstall

all: build/lib/libtdk.so

clean:
	rm -rf build;

install: build/lib/libtdk.so src/tdk.h
	mkdir -p ${LIBPATH} ${CPATH} ${MANPATH}/man3;
	cp ${<} ${LIBPATH};
	cp $(word 2, ${^}) ${CPATH};
	for m in $(wildcard man/*); \
	do \
		if [[ -L $${m} ]]; \
		then \
			cp -d $${m} ${MANPATH}/man3; \
		else \
			f=$${m##*/}; \
			sed s/\$${VERSION}/${VERSION}/ $${m} > ${MANPATH}/man3/$${f}; \
		fi \
	done

uninstall:
	rm -f ${LIBPATH}/libtdk.so ${CPATH}/tdk.h ${MANPATH}/man3/{tdk.3,tdk_*.3};

build/obj/tdk.o: src/tdk.c src/tdk.h
	mkdir -p build/obj;
	${CC} ${CFLAGS} -fPIC -c -o ${@} ${<};

build/lib/libtdk.so: build/obj/tdk.o
	mkdir -p build/lib;
	${CC} ${CFLAGS} -shared -o ${@} ${<};
