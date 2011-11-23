include Makefile.rules


OBJS=main.o\
face.o\
mesh.o\
quadric.o\
vertex.o\
vertex_pair.o\
vertex_pair_heap.o\
../common/mesh_file_helper.o\
../common/ply.o\


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
	rm -rf $(PROG).exe
