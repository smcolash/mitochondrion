include ../config.mak

vpath %.cxx ../../../source/examples/Example

SOURCES = \
    main.cxx \
    A.cxx \
    B.cxx \
    C.cxx \
    D.cxx

PORT = 8765

OBJECTS = $(SOURCES:.cxx=.o)

all : example

debug : example
	@export PATH=../lib:"${PATH}" ; gdb ./example.exe

test : example
	@export PATH=../lib:"${PATH}" ; ./example.exe $(PORT)

example : $(OBJECTS)
	$(CPP) -o example $(OBJECTS) \
		-L../lib -lMitoKernel -lMitoLog -lMitoStat \
		-Wl,--subsystem,windows

.PHONY : clean
clean :
	rm -f *.o
	rm -f example
