include ../config.mak

vpath %.cxx ../../../Example

SOURCES = \
    main.cxx \
    A.cxx \
    B.cxx \
    C.cxx

OBJECTS = $(SOURCES:.cxx=.o)

all : example

debug : example
	@export LD_LIBRARY_PATH=../lib:"${LD_LIBRARY_PATH}" ; gdb ./example

test : example
	@export LD_LIBRARY_PATH=../lib:"${LD_LIBRARY_PATH}" ; ./example 8765

example : $(OBJECTS)
	$(CPP) -o example $(OBJECTS) -L../lib -lMitoKernel -lMitoLog -lMitoStat

.PHONY : clean
clean :
	rm -f *.o
	rm -f example
