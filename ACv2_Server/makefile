# This file demonstrates how to compile the SAMPAC project on Linux.
#
# To compile SAMPAC do:
#
# make SAMPAC
#

GPP = g++-4.4
GCC = gcc
AC_OUTFILE = "bin/sampac.so"

COMPILE_FLAGS = -m32 -fPIC -c -O2 -w -D LINUX -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -D PROJECT_NAME=\"sampac\" -D SAMPGDK_AMALGAMATION -DSAMPGDK_CPP_WRAPPERS -D SUBHOOK_STATIC -DCURL_STATICLIB -I ./../Shared/
LIBS = -L./../Shared/libs -static -static-libgcc -lc -lgcc -ldl -lboost_system -lboost_thread -lboost_filesystem -lboost_chrono -lboost_date_time -lrt -lsubhook -lcurl -lssl -lcrypto

LDFLAGS = -shared -lpthread

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
	$(GCC) $(SAMPAC) ./GDK/*.c
	$(GPP) $(SAMPAC) *.cpp
	$(GPP) $(SAMPAC) ./Network/*.cpp
	$(GPP) $(SAMPAC) ../Shared/MD5_Info/*.cpp
	$(GPP) $(SAMPAC) ../Shared/Network/*.cpp
	$(GPP) $(SAMPAC) ../Shared/SelfUpdater/*.cpp
	mkdir -p "bin"
	$(GPP) -m32 -O2 -fshort-wchar -static -o $(AC_OUTFILE) *.o $(LDFLAGS) $(LIBS)
