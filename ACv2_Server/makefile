# This file demonstrates how to compile the SAMPAC project on Linux.
#
# To compile SAMPAC do:
#
# make SAMPAC
#

GPP = g++-4.4
GCC = gcc
AC_OUTFILE = "bin/sampac.so"

COMPILE_FLAGS = -m32 -fPIC -c -O2 -w -D LINUX -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -D NO_BOOST_DATE_TIME_INLINE -D PROJECT_NAME=\"sampac\" -I ./../Shared/RakNet/ -I ./../Shared/
LIBS = -L./../Shared/libs -static -g -static-libgcc -lc -lgcc -ldl -lsampgdk -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lboost_date_time -lcppnetlib-uri -lcppnetlib-client-connections -lrt

LDFLAGS = -g -shared -lpthread

SAMPAC = -D SAMPAC $(COMPILE_FLAGS)

all: SAMPAC

clean:
	-rm -f *~ *.o *.so

quick:
	$(GPP) -m32 -O2 -fshort-wchar -static -o $(AC_OUTFILE) *.o $(LDFLAGS) $(LIBS)

quickcomp:

	$(GPP) $(SAMPAC) *.cpp
	quick

SAMPAC: clean
	$(GPP) $(SAMPAC) ./SDK/samp-sdk/*.cpp
	$(GPP) $(SAMPAC) *.cpp
	$(GPP) $(SAMPAC) ./Network/*.cpp
	$(GPP) $(SAMPAC) ../Shared/MD5_Info/*.cpp
	$(GPP) $(SAMPAC) ../Shared/Network/*.cpp
	$(GPP) $(SAMPAC) ../Shared/RakNet/*.cpp
	$(GPP) -m32 -O2 -fshort-wchar -static -o $(AC_OUTFILE) *.o $(LDFLAGS) $(LIBS)

