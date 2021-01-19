include ../config.mak

vpath %.cxx ../../../MitoStat

all : libraries install

.PHONY : clean
clean :
	rm -f *.o
	rm -f *.a
	rm -f libMitoStat.so
	rm -f ../lib/libMitoStat.so

MITOSTAT_SRC = \
    Statistics.cxx \
    Sequence.cxx

MITOSTAT_OBJ = $(MITOSTAT_SRC:.cxx=.o)

libMitoStat.so : $(MITOSTAT_OBJ)
	g++ $(LDFLAGS) -o libMitoStat.so \
      $(MITOSTAT_OBJ)

../lib/libMitoStat.so : libMitoStat.so
	mkdir --parents ../lib
	cp -f libMitoStat.so ../lib

libraries :: libMitoStat.so
install :: libraries ../lib/libMitoStat.so
