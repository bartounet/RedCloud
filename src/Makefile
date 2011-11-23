
MAKE=gmake

all: FINAL

DEBUG:
	cd VertexRecolor && ./configure DEBUG && $(MAKE) clean && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && $(MAKE) clean && $(MAKE) && cd ..
	cd Texturer && ./configure DEBUG && $(MAKE) clean && $(MAKE) && cd ..
	cd Cleaner && ./configure DEBUG && $(MAKE) clean && $(MAKE) && cd ..

RELEASE:
	cd VertexRecolor && ./configure RELEASE && $(MAKE) clean && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && $(MAKE) clean && $(MAKE) && cd ..
	cd Texturer && ./configure RELEASE && $(MAKE) clean && $(MAKE) && cd ..
	cd Cleaner && ./configure RELEASE && $(MAKE) clean && $(MAKE) && cd ..

FINAL:
	cd VertexRecolor && ./configure FINAL && $(MAKE) clean && $(MAKE) && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && $(MAKE) clean && $(MAKE) && cd ..
	cd Texturer && ./configure FINAL && $(MAKE) clean && $(MAKE) && cd ..
	cd Cleaner && ./configure FINAL && $(MAKE) clean && $(MAKE) && cd ..

clean:
	cd VertexRecolor && $(MAKE) clean && $(MAKE) clean && cd ..
	cd QuadricsBasedMeshSimplifier && $(MAKE) clean && $(MAKE) clean && cd ..
	cd Texturer && $(MAKE) clean && $(MAKE) clean && cd ..
	cd Cleaner && $(MAKE) clean && $(MAKE) clean && cd ..

cleanVCPP:
	rm -rf Release
	rm -rf Final
	rm -rf Debug
	cd VertexRecolor && ./configure DEBUG && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd VertexRecolor && ./configure RELEASE && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd VertexRecolor && ./configure FINAL && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd Texturer && ./configure DEBUG && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd Texturer && ./configure RELEASE && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd Texturer && ./configure FINAL && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd Cleaner && ./configure DEBUG && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd Cleaner && ./configure RELEASE && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..
	cd Cleaner && ./configure FINAL && $(MAKE) clean && $(MAKE) cleanVCPP && cd ..

distclean: cleanVCPP clean
	cd VertexRecolor && ./configure DEBUG && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd VertexRecolor && ./configure RELEASE && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd VertexRecolor && ./configure FINAL && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd Texturer && ./configure DEBUG && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd Texturer && ./configure RELEASE && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd Texturer && ./configure FINAL && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd Cleaner && ./configure DEBUG && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd Cleaner && ./configure RELEASE && $(MAKE) clean && $(MAKE) distclean && cd ..
	cd Cleaner && ./configure FINAL && $(MAKE) clean && $(MAKE) distclean && cd ..
