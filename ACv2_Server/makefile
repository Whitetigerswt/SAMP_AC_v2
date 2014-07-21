# This file demonstrates how to compile the SAMPAC project on Linux.
#
# To compile SAMPAC do:
#
# make SAMPAC
#

GPP = g++
GCC = gcc
AC_OUTFILE = "bin/sampac.so"

COMPILE_FLAGS = -m32 -fPIC -c -O2 -w -D LINUX -D PROJECT_NAME=\"sampac\" -I ./../Shared/RakNet/
LIBS = -L./../Shared/libs -static -g -lsampgdk
LDFLAGS = -shared -g -lpthread

SAMPAC = -D SAMPAC $(COMPILE_FLAGS)

all: SAMPAC

clean:
	-rm -f *~ *.o *.so

SAMPAC: clean
	$(GPP) $(SAMPAC) ./SDK/samp-sdk/*.cpp
	$(GPP) $(SAMPAC) *.cpp
	$(GPP) $(SAMPAC) ./Network/*.cpp
	$(GPP) $(SAMPAC) ../Shared/Network/*.cpp
	$(GPP) $(SAMPAC) ../Shared/RakNet/*.cpp
	$(GPP) -m32 -O2 -fshort-wchar -static -o $(AC_OUTFILE) *.o $(LDFLAGS) $(LIBS)

