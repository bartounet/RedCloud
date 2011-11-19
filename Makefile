
all: FINAL

DEBUG:
	cd VertexRecolor && ./configure DEBUG && gmake && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && gmake && cd ..

RELEASE:
	cd VertexRecolor && ./configure RELEASE && gmake && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && gmake && cd ..

FINAL:
	cd VertexRecolor && ./configure FINAL && gmake && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && gmake && cd ..

clean:
	cd VertexRecolor && gmake clean && cd ..
	cd QuadricsBasedMeshSimplifier && gmake clean && cd ..

cleanVCPP:
	rm -rf Release
	rm -rf Final
	rm -rf Debug
	cd VertexRecolor && ./configure DEBUG && gmake cleanVCPP && cd ..
	cd VertexRecolor && ./configure RELEASE && gmake cleanVCPP && cd ..
	cd VertexRecolor && ./configure FINAL && gmake cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && gmake cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && gmake cleanVCPP && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && gmake cleanVCPP && cd ..

distclean: cleanVCPP clean
	cd VertexRecolor && ./configure DEBUG && gmake distclean && cd ..
	cd VertexRecolor && ./configure RELEASE && gmake distclean && cd ..
	cd VertexRecolor && ./configure FINAL && gmake distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure DEBUG && gmake distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure RELEASE && gmake distclean && cd ..
	cd QuadricsBasedMeshSimplifier && ./configure FINAL && gmake distclean && cd ..
