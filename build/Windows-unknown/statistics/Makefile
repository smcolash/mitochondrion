include ../config.mak

vpath %.cxx ../../../statistics

SOURCES = \
    main.cxx

OBJECTS = $(SOURCES:.cxx=.obj)

all : example

debug : example
	@export PATH=../lib:"${PATH}" ; gdb ./example.exe

test : example
	@export PATH=../lib:"${PATH}" ; ./example.exe

#
# import library problem...
#
example : $(OBJECTS)
	$(LD) /out:example.exe \
        $(OBJECTS) \
        /LIBPATH:../lib MitoStat.lib

.PHONY : clean
clean :
	rm -f *.obj
	rm -f example

rebuild : clean all
