include Makefile.rules


OBJS=main.o\
face.o\
mesh.o\
quadric.o\
vertex.o\
vertex_pair.o\
vertex_pair_heap.o\
../VertexRecolor/mesh_file_helper.o\
../VertexRecolor/ply.o\


all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(PREPROCS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(PREPROCS) -c $< -o $@	

clean:
	rm -rf $(OBJS)

cleanVCPP:
	rm -rf Release
	rm -rf Final
	rm -rf Debug

distclean: cleanVCPP clean
	rm -rf Makefile.rules
	rm -rf $(PROG)

