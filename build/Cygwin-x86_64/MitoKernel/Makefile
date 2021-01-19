include ../config.mak

vpath %.cxx ../../../source/MitoKernel

all : libraries install

.PHONY : clean
clean ::
	rm -f *.o
	rm -f *.a
	rm -f libMitoKernel.dll
	rm -f ../lib/libMitoKernel.dll

MITOKERNEL_SRC = \
        Actor.cxx \
        BehaviorNode.cxx \
        ChoicePoint.cxx \
        Event.cxx \
        FdEvent.cxx \
        FdPort.cxx \
        FdQueue.cxx \
        Port.cxx \
        Queue.cxx \
        State.cxx \
        StateList.cxx \
        TimerEvent.cxx \
        TimerPort.cxx \
        TimerQueue.cxx \
        Transition.cxx \
        TransitionList.cxx

MITOKERNEL_OBJ = $(MITOKERNEL_SRC:.cxx=.o)

libMitoKernel.dll : $(MITOKERNEL_OBJ)
	g++ -shared -o libMitoKernel.dll \
      -Wl,--export-all-symbols \
      -Wl,--enable-auto-import \
      -Wl,--no-whole-archive -L../lib -lMitoLog \
      $(MITOKERNEL_OBJ)

../lib/libMitoKernel.dll : libMitoKernel.dll
	mkdir --parents ../lib
	cp -f libMitoKernel.dll ../lib

libraries :: libMitoKernel.dll
install :: libraries ../lib/libMitoKernel.dll

