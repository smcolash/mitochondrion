include ../config.mak

vpath %.cxx ../../../source/examples/statistics

SOURCES = \
    main.cxx

OBJECTS = $(SOURCES:.cxx=.o)

all : example

debug : example
	@export PATH=../lib:"${PATH}" ; gdb ./example.exe

test : example
	@export PATH=../lib:"${PATH}" ; ./example.exe

example : $(OBJECTS)
	$(CPP) -o example $(OBJECTS) \
		-L../lib -lMitoStat \
		-Wl,--subsystem,windows

.PHONY : clean
clean :
	rm -f *.o
	rm -f example
