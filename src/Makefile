
MAKE=make
SHELL=/bin/sh

all: FINAL

DEBUG:
	cd VertexRecolor && $(SHELL) configure DEBUG && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure DEBUG && $(MAKE) && cd ..
	cd Texturer && $(SHELL) configure DEBUG && $(MAKE) && cd ..

RELEASE:
	cd VertexRecolor && $(SHELL) configure RELEASE && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure RELEASE && $(MAKE) && cd ..
	cd Texturer && $(SHELL) configure RELEASE && $(MAKE) && cd ..

FINAL:
	cd VertexRecolor && $(SHELL) configure FINAL && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure FINAL && $(MAKE) && cd ..
	cd Texturer && $(SHELL) configure FINAL && $(MAKE) && cd ..

clean:
	cd VertexRecolor && $(MAKE) clean && cd ..
	cd QuadricsBasedMeshSimplifier && $(MAKE) clean && cd ..
	cd Texturer && $(MAKE) clean && cd ..

cleanVCPP:
	rm -rf Release
	rm -rf Final
	rm -rf Debug
	cd VertexRecolor && $(SHELL) configure DEBUG && $(MAKE) cleanVCPP && cd ..
	cd VertexRecolor && $(SHELL) configure RELEASE && $(MAKE) cleanVCPP && cd ..
	cd VertexRecolor && $(SHELL) configure FINAL && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure DEBUG && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure RELEASE && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure FINAL && $(MAKE) cleanVCPP && cd ..

distclean: cleanVCPP clean
	cd VertexRecolor && $(SHELL) configure DEBUG && $(MAKE) distclean && cd ..
	cd VertexRecolor && $(SHELL) configure RELEASE && $(MAKE) distclean && cd ..
	cd VertexRecolor && $(SHELL) configure FINAL && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure DEBUG && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure RELEASE && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && $(SHELL) configure FINAL && $(MAKE) distclean && cd ..
