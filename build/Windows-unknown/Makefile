SUBDIRS = \
    MitoLog \
    MitoStat \
    MitoKernel \
    Example

SUBDIRS = MitoStat

PORT = 8765

.PHONY: subdirs $(SUBDIRS)
subdirs : $(SUBDIRS)

all : subdirs

test : all
	cd Example && make test PORT=$(PORT)

$(SUBDIRS) :
	$(MAKE) -C $@

rebuild : clean all

clean ::
	for dir in $(SUBDIRS); do ( cd $$dir ; $(MAKE) clean ); done

