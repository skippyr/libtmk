CC:=cc
INCPATH:=/usr/local/include
LIBPATH:=/usr/local/lib
MAN3PATH:=/usr/local/man/man3

.PHONY: all install uninstall

all: libtdk.so

install: all
	mkdir -p ${LIBPATH} ${INCPATH} ${MAN3PATH}
	mv libtdk.so ${LIBPATH}
	cp tdk.h ${INCPATH}
	for m in $(wildcard man/*);\
	do\
		sed "s/\$${VERSION}/v6.0.0/; s/\$${PACKAGE}/mistureba/" $${m} >\
		     ${MAN3PATH}/$${m##*/};\
	done

uninstall:
	rm -f ${LIBPATH}/libtdk.so ${INCPATH}/tdk.h ${MAN3PATH}/{tdk.3,tdk_*.3}

libtdk.so: tdk.c tdk.h
	${CC} -std=c99 -pedantic -Os -Wall -otdk.o -c -fPIC ${<}
	${CC} -o${@} -shared tdk.o
	rm tdk.o
