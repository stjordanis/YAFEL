subdirs = src test #doc #apps 

ifndef YAFELDIR
 export YAFELDIR=$(shell pwd)
 $(warning Your YAFELDIR is not defined. Set to "$(YAFELDIR)".)
endif

include $(YAFELDIR)/common.mk

all: $(subdirs)

$(subdirs):
	$(MAKE) -C $@ all

clean:
	for dir in $(subdirs); do $(MAKE) -C $$dir clean; done
	rm $(YAFELDIR)/lib/$(LIB)

runtests: all test
	make -C test runtests

.PHONY: all $(subdirs) runtests
