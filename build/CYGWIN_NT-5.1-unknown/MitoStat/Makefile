include ../config.mak

vpath %.cxx ../../../source/MitoStat

all : libraries install

.PHONY : clean
clean :
	rm -f *.o
	rm -f *.a
	rm -f libMitoStat.dll
	rm -f ../lib/libMitoStat.dll

MITOSTAT_SRC = \
    Statistics.cxx \
    Sequence.cxx

MITOSTAT_OBJ = $(MITOSTAT_SRC:.cxx=.o)

libMitoStat.dll : $(MITOSTAT_OBJ)
	g++ -shared -o libMitoStat.dll \
      -Wl,--export-all-symbols \
      -Wl,--enable-auto-import \
      $(MITOSTAT_OBJ)

../lib/libMitoStat.dll : libMitoStat.dll
	mkdir --parents ../lib
	cp -f libMitoStat.dll ../lib

libraries :: libMitoStat.dll
install :: libraries ../lib/libMitoStat.dll
