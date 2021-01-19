SUBDIRS = \
    MitoLog \
    MitoStat \
    MitoKernel \
    Example

.PHONY: subdirs $(SUBDIRS)
subdirs : $(SUBDIRS)

all : subdirs

test : all
	cd Example && make test

$(SUBDIRS) :
	$(MAKE) -C $@

rebuild : clean all

clean ::
	for dir in $(SUBDIRS); do ( cd $$dir ; $(MAKE) clean ); done

