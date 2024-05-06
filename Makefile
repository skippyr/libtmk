VERSION:=v11.0.0
CC:=cc
CFLAGS:=-std=c99 -Wpedantic -Wall -Wextra -Wno-unused-result -Os
CPATH:=~/.local/share/include
LIBPATH:=~/.local/share/lib
MANPATH:=~/.local/share/man
SHELL:=bash

.PHONY: all clean install uninstall tools

all: build/lib/libtdk.so

clean:
	rm -rf build;

install: build/lib/libtdk.so src/tdk.h
	mkdir -p ${LIBPATH} ${CPATH} ${MANPATH}/man3;
	cp ${<} ${LIBPATH};
	cp $(word 2, ${^}) ${CPATH};
	for manual in $(wildcard man/*); \
	do \
		if [[ -L $${manual} ]]; \
		then \
			cp -d $${manual} ${MANPATH}/man3; \
		else \
			name=$${manual##*/}; \
			sed s/\$${VERSION}/${VERSION}/ $${manual} > ${MANPATH}/man3/$${name}; \
		fi \
	done

uninstall:
	rm -f ${LIBPATH}/libtdk.so ${CPATH}/tdk.h ${MANPATH}/man3/{tdk.3,tdk_*.3};

tools: build/tools/key-dump

build/obj/tdk.o: src/tdk.c src/tdk.h
	mkdir -p build/obj;
	${CC} ${CFLAGS} -fPIC -c -o ${@} ${<};

build/lib/libtdk.so: build/obj/tdk.o
	mkdir -p build/lib;
	${CC} ${CFLAGS} -shared -o ${@} ${<};

build/tools/key-dump: tools/key-dump.c
	mkdir -p build/tools;
	${CC} ${CFLAGS} -o ${@} ${<};
