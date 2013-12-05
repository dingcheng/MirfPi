#############################################################################
#
# Makefile for librf24 on Raspberry Pi
#
# License: GPL (General Public License)
# Author:  gnulnulf <arco@appeltaart.mine.nu>
# Date:    2013/02/07 (version 1.0)
#
# Description:
# ------------
# use make all and mak install to install the library 
# You can change the install directory by editing the LIBDIR line
#
LIBDIR=/usr/local/lib
LIBNAME=libmirf24.so.1.0


# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s

# make all
all: libmirf24

# Make the library
libmirf24: Mirf.o gpio.o spi.o compatibility.o
	g++ -shared -Wl,-soname,libmirf24.so.1 ${CCFLAGS}  -o ${LIBNAME} compatibility.o gpio.o spi.o Mirf.o

# Library parts
Mirf.o: Mirf.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c Mirf.cpp

gpio.o: gpio.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c gpio.cpp

spi.o: spi.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c spi.cpp

compatibility.o: compatibility.c
	gcc -Wall -fPIC  ${CCFLAGS} -c compatibility.c

pingtest: pingtest.cpp
	g++ ${CCFLAGS} -L./  -lmirf24 $@.cpp -o $@

# clear build files
clean:
	rm -rf *o ${LIBNAME}

# Install the library to LIBPATH
install: 
	cp libmirf24.so.1.0 ${LIBDIR}/${LIBNAME}
	ln -sf ${LIBDIR}/${LIBNAME} ${LIBDIR}/libmirf24.so.1
	ln -sf ${LIBDIR}/${LIBNAME} ${LIBDIR}/libmirf24.so
	ldconfig

