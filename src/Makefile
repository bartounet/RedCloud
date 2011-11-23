
MAKE=gmake

all: FINAL

DEBUG:
	cd VertexRecolor && ./configure DEBUG && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && $(MAKE) && cd ..
	cd Texturer && ./configure DEBUG && $(MAKE) && cd ..

RELEASE:
	cd VertexRecolor && ./configure RELEASE && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && $(MAKE) && cd ..
	cd Texturer && ./configure RELEASE && $(MAKE) && cd ..

FINAL:
	cd VertexRecolor && ./configure FINAL && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && $(MAKE) && cd ..
	cd Texturer && ./configure FINAL && $(MAKE) && cd ..

clean:
	cd VertexRecolor && $(MAKE) clean && cd ..
	cd QuadricsBasedMeshSimplifier && $(MAKE) clean && cd ..
	cd Texturer && $(MAKE) clean && cd ..

cleanVCPP:
	rm -rf Release
	rm -rf Final
	rm -rf Debug
	cd VertexRecolor && ./configure DEBUG && $(MAKE) cleanVCPP && cd ..
	cd VertexRecolor && ./configure RELEASE && $(MAKE) cleanVCPP && cd ..
	cd VertexRecolor && ./configure FINAL && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && $(MAKE) cleanVCPP && cd ..

distclean: cleanVCPP clean
	cd VertexRecolor && ./configure DEBUG && $(MAKE) distclean && cd ..
	cd VertexRecolor && ./configure RELEASE && $(MAKE) distclean && cd ..
	cd VertexRecolor && ./configure FINAL && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && $(MAKE) distclean && cd ..
