
MAKE=gmake

all: install

install:
	cd src && $(MAKE) RELEASE && cd ..

distclean:
	cd src && $(MAKE) distclean && cd ..