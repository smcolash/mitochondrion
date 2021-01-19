include ../config.mak

vpath %.cxx ../../../MitoStat

all : libraries install

.PHONY : clean
clean :
	rm -f *.obj
	rm -f MitoStat.{dll,lib,ilk,pdb,exp}
	rm -f ../lib/MitoStat.{dll,lib}

DEFINES += /D __DLL_EXPORT__

MITOSTAT_SRC = \
    Statistics.cxx \
    Sequence.cxx

MITOSTAT_OBJ = $(MITOSTAT_SRC:.cxx=.obj)

MitoStat.dll : $(MITOSTAT_OBJ)
	$(AR) /out:MitoStat.dll \
      $(MITOSTAT_OBJ)

../lib/MitoStat.dll : MitoStat.dll
	mkdir --parents ../lib
	cp -f MitoStat.{dll,lib} ../lib

libraries :: MitoStat.dll
install :: libraries ../lib/MitoStat.dll
