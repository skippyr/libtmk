include config.mk

.PHONY: all clean install uninstall

all: build/libtdk.so

clean:
	rm -rf build;

install: all
	mkdir -p ${INSTALLATION_MAN3_PATH} ${INSTALLATION_INCLUDE_PATH}        \
	         ${INSTALLATION_LIBRARY_PATH};
	for manual in $(wildcard man/*);                                       \
	do                                                                     \
		sed "s/\$${LIBRARY_VERSION}/${LIBRARY_VERSION}/;               \
		     s/\$${LIBRARY_PACKAGE}/${LIBRARY_PACKAGE}/" $${manual} >  \
		     ${INSTALLATION_MAN3_PATH}/$${manual##*/};                 \
	done
	cp src/tdk.h ${INSTALLATION_INCLUDE_PATH};
	cp build/libtdk.so ${INSTALLATION_LIBRARY_PATH};

uninstall:
	rm -f ${INSTALLATION_MAN3_PATH}/{tdk.3,tdk_*.3}                        \
	      ${INSTALLATION_INCLUDE_PATH}/tdk.h                               \
	      ${INSTALLATION_LIBRARY_PATH}/libtdk.so;

build/libtdk.so: build/tdk.o
	${COMPILER} -shared -o${@} ${^};

build/tdk.o: src/tdk.c src/tdk.h
	mkdir -p build;
	${COMPILER} ${COMPILER_OPTIONS} -c -fPIC -o${@} ${<};
