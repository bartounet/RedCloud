include Makefile.rules


OBJS=main.o\
Color.o\
Face.o\
Plan.o\
Point2d.o\
Point2dColor.o\
Point3d.o\
Point3dColor.o\
Texture.o\
World.o\
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
