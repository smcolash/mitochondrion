include ../config.mak

vpath %.cxx ../../../source/MitoLog
vpath %.js ../../../source/MitoLog
vpath %.ico ../../../source/MitoLog
vpath %.css ../../../source/MitoLog

all : libraries install

.PHONY : clean
clean :
	rm -f association.data
	rm -f history.data
	rm -f excanvas.data
	rm -f favicon.data
	rm -f style.data
	rm -f *.o
	rm -f *.a
	rm -f libMitoLog.dll
	rm -f ../lib/libMitoLog.dll

MITOLOG_SRC = \
        MitoLogRecord.cxx \
        MitoLog.cxx \
        Mutex.cxx \
        Url.cxx \
        Component.cxx \
        WebServer.cxx

association.data : association.js
history.data : history.js
excanvas.data : excanvas.js
favicon.data : favicon.ico
style.data : style.css

MitoLog.o :: association.data history.data excanvas.data favicon.data style.data

MITOLOG_OBJ = $(MITOLOG_SRC:.cxx=.o)

libMitoLog.dll : $(MITOLOG_OBJ)
	g++ -shared -o libMitoLog.dll \
      -Wl,--export-all-symbols \
      -Wl,--enable-auto-import \
      $(MITOLOG_OBJ)

../lib/libMitoLog.dll : libMitoLog.dll
	mkdir --parents ../lib
	cp -f libMitoLog.dll ../lib

libraries : libMitoLog.dll
install :: libraries ../lib/libMitoLog.dll

