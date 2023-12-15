include config.mk

all: libtdk.so

clean:
	rm -f tdk.o libtdk.so

install: all
	mkdir -p ${LIBPATH} ${MAN3PATH}
	for m in $(wildcard man/*); do\
		sed 's/MANDATE/${VERSION}/' $${m} > ${MAN3PATH}/$${m##*/};\
	done
	cp tdk.h ${INCPATH}
	mv libtdk.so ${LIBPATH}

libtdk.so: tdk.o
	${CC} -shared -o${@} ${^}

tdk.o: tdk.c
	${CC} ${CFLAGS} -c -fPIC -o${@} ${^}

uninstall:
	rm -f ${MAN3PATH}/{tdk.3,tdk_*.3} ${INCPATH}/tdk.h ${LIBPATH}/libtdk.so


.PHONY: all clean install uninstall
