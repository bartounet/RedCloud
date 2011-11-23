
MAKE=gmake

all: install

install:
	cd src && $(MAKE) FINAL && cd ..

distclean:
	cd src && $(MAKE) distclean && cd ..