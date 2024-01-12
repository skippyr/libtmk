include config.mk

.PHONY: all clean install uninstall

all: libtdk.so

clean:
	rm -f libtdk.so tdk.o;

install: all
	mkdir -p ${LIBPATH} ${INCPATH} ${MAN3PATH};
	cp libtdk.so ${LIBPATH};
	cp tdk.h ${INCPATH};
	for m in $(wildcard man/*);\
	do\
		sed "s/\$${VERSION}/${VERSION}/" $${m} > ${MAN3PATH}/$${m##*/};\
	done

uninstall:
	rm -f ${LIBPATH}/libtdk.so ${INCPATH}/tdk.h ${MAN3PATH}/{tdk.3,tdk_*.3};

libtdk.so: tdk.o
	${CC} -o${@} -shared ${^};

tdk.o: tdk.c tdk.h
	${CC} ${CFLAGS} -o${@} -c -fPIC ${<};
