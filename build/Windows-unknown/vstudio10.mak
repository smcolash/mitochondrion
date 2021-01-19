#
# Use Microsoft Visual Studio 10 C++ to build binaries from within Cygwin
#

#
# set the base path to the Visual Studio installation
#
__BASE = $(shell cygpath --unix 'C:\Program Files\Microsoft Visual Studio 10.0')

#
# set the system include path
#
INCLUDE := $(shell cygpath --dos --short-name '$(__BASE)/VC/INCLUDE')
INCLUDE := ${INCLUDE};$(shell cygpath --dos --short-name 'C:\Program Files\Microsoft SDKs\Windows\v7.0A\include')
export INCLUDE

#
# set the system link path
#
LIB := $(shell cygpath --dos --short-name '$(__BASE)/VC/LIB')
LIB := ${LIB};$(shell cygpath --dos --short-name 'C:\Program Files\Microsoft SDKs\Windows\v7.0A\lib')
export LIB

#
# set the system runtime library path
#
LIBPATH := $(shell cygpath --dos --short-name 'C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319')
LIBPATH := ${LIBPATH};$(shell cygpath --dos --short-name 'C:\WINDOWS\Microsoft.NET\Framework\v3.5')
LIBPATH := ${LIBLATH};$(shell cygpath --dos --short-name 'C:\Program Files\Microsoft Visual Studio 10.0\VC\LIB')
export LIBPATH

#
# update the executable search path
#
PATH := $(__BASE)/Common7/IDE:${PATH}
PATH := $(__BASE)/VC/BIN:${PATH}

