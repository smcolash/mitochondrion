include ../config.mak

vpath %.cxx ../../../MitoLog
vpath %.js ../../../MitoLog
vpath %.ico ../../../MitoLog

all : libraries install

.PHONY : clean
clean :
	rm -f support.data
	rm -f excanvas.data
	rm -f favicon.data
	rm -f *.o
	rm -f *.a
	rm -f libMitoLog.so
	rm -f ../lib/libMitoLog.so

MITOLOG_SRC = \
        MitoLogRecord.cxx \
        MitoLog.cxx \
        Mutex.cxx \
        Url.cxx \
        Component.cxx \
        WebServer.cxx

association.data : association.js
support.data : support.js
excanvas.data : excanvas.js
favicon.data : favicon.ico

MitoLog.o :: association.data support.data excanvas.data favicon.data

MITOLOG_OBJ = $(MITOLOG_SRC:.cxx=.o)

libMitoLog.so : $(MITOLOG_OBJ)
	g++ $(LDFLAGS) -o libMitoLog.so \
      -lrt \
      $(MITOLOG_OBJ)

../lib/libMitoLog.so : libMitoLog.so
	mkdir --parents ../lib
	cp -f libMitoLog.so ../lib

libraries : libMitoLog.so
install :: libraries ../lib/libMitoLog.so

