include ../vstudio10.mak

# ----------------------------------------------------------------------

CPP = cl \
    /nologo

CPPFLAGS = \
    /clr \
    /W3 \
    /WX- \
	/c

LD = link \
    /nologo

AR = link \
    /nologo \
    /dll \
    /dynamicbase \
    /fixed:NO \
    /nxcompat

#
# settings for build
#
INCLUDES = /I ../../../include /I .

DEFINES = \
    /D "WIN32" \
    /D "_WINDLL" \
    /D "_UNICODE" \
    /D "UNICODE"

%.obj : %.cxx
	$(CPP) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEFINES) /c $<

%.data : %.js
	cat $< | xxd -i > $@

%.data : %.ico
	xxd -i < $< > $@

