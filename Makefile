
install:
	cd src && make && cd ..
	cp -fv src/VertexRecolor/vr_final Bin/Linux/
	cp -fv src/QuadricsBasedMeshSimplifier/qbms_final Bin/Linux/
	cp -fv src/Texturer/bin/Release/Texturer Bin/Linux/
