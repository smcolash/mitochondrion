KERNEL := $(shell uname -s)
MACHINE := $(shell uname -i)
TARGET := $(KERNEL)-$(MACHINE)

PORT = 8765

SUBDIRS = \
    $(TARGET)

.PHONY: subdirs $(SUBDIRS)
subdirs : $(SUBDIRS)

all : subdirs

$(SUBDIRS) :
	$(MAKE) -C $@

rebuild : clean all

clean ::
	for dir in $(SUBDIRS); do ( cd $$dir ; $(MAKE) clean ); done

test : all
	cd $(TARGET)/Example && make test PORT=$(PORT)
