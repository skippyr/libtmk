VERSION:=v7.0.0
CC:=cc
CFLAGS:=-std=c99 -pedantic -Os -Wall -Wextra -ltdk
INCPATH:=/usr/local/include
LIBPATH:=/usr/local/lib
MAN3PATH:=/usr/local/man/man3
SHELL:=bash

.PHONY: all clean install uninstall

all: out/libtdk.so

clean:
	rm -rf out;

install: out/libtdk.so
	mkdir -p ${LIBPATH} ${INCPATH} ${MAN3PATH};
	cp out/libtdk.so ${LIBPATH};
	cp src/tdk.h ${INCPATH};
	for m in $(wildcard man/*); \
	do \
		[[ -L $${m} ]] && cp -r $${m} ${MAN3PATH} || \
			sed "s/\$${VERSION}/${VERSION}/" $${m} > ${MAN3PATH}/$${m##*/}; \
	done

uninstall:
	rm -f ${LIBPATH}/libtdk.so ${INCPATH}/tdk.h ${MAN3PATH}/{tdk.3,tdk_*.3};

out/libtdk.so: out/tdk.o
	${CC} -o${@} -shared ${^};

out/tdk.o: src/tdk.c src/tdk.h
	mkdir -p out;
	${CC} ${CFLAGS} -o${@} -c -fPIC ${<};
