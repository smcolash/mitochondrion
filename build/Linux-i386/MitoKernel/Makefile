include ../config.mak

vpath %.cxx ../../../MitoKernel

all : libraries install

.PHONY : clean
clean ::
	rm -f *.o
	rm -f *.a
	rm -f libMitoKernel.so
	rm -f ../lib/libMitoKernel.so

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

libMitoKernel.so : $(MITOKERNEL_OBJ)
	g++ $(LDFLAGS) -o libMitoKernel.so \
        -L../lib -lMitoLog \
        $(MITOKERNEL_OBJ)

../lib/libMitoKernel.so : libMitoKernel.so
	mkdir --parents ../lib
	cp -f libMitoKernel.so ../lib

libraries :: libMitoKernel.so
install :: libraries ../lib/libMitoKernel.so

