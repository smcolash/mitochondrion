SUBDIRS = \
    MitoLog \
    MitoStat \
    MitoKernel \
    Example \
    statistics

.PHONY: subdirs $(SUBDIRS)
subdirs : $(SUBDIRS)

all : subdirs

PORT = 8765

test : all
	cd Example && make test PORT=$(PORT)

$(SUBDIRS) :
	$(MAKE) -C $@

rebuild : clean all

clean ::
	for dir in $(SUBDIRS); do ( cd $$dir ; $(MAKE) clean ); done

