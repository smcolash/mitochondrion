include ../config.mak

vpath %.cxx ../../../MitoLog
vpath %.js ../../../MitoLog
vpath %.ico ../../../MitoLog

all : libraries install

.PHONY : clean
clean :
	rm -f association.data
	rm -f support.data
	rm -f excanvas.data
	rm -f favicon.data
	rm -f *.obj
	rm -f *.a
	rm -f MitoLog.{dll,lib,ilk,pdb,exp}
	rm -f ../lib/MitoLog.{dll,lib}

DEFINES += /D __DLL_EXPORT__

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

MitoLog.obj :: association.data support.data excanvas.data favicon.data

MITOLOG_OBJ = $(MITOLOG_SRC:.cxx=.obj)

MitoLog.dll : $(MITOLOG_OBJ)
	$(AR) /out:MitoLog.dll \
      $(MITOLOG_OBJ)

../lib/MitoLog.dll : MitoLog.dll
	mkdir --parents ../lib
	cp -f MitoLog.{dll,lib} ../lib

libraries : MitoLog.dll
install :: libraries ../lib/MitoLog.dll
