#
# settings for build
#
AR = ar
LD = ld
CPP = g++

CFLAGS = \
    -O2 \
    -g3 \
    -ggdb \
    -Wno-deprecated \
    -Wall \
    -Werror

LDFLAGS = \
    -shared \
    -ggdb \
    -Wl,--no-whole-archive

INCLUDES = -I../../../include -I./

%.o : %.cxx
	$(CPP) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) -c $<

%.data : %.js
	xxd -i < $< > $@

%.data : %.ico
	xxd -i < $< > $@

