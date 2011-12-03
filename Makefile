
MAKE=make

all: install

install:
	cd src && $(MAKE) RELEASE && cd ..

distclean:
	cd src && $(MAKE) distclean && cd ..
