CC = gcc
INC_DIR = include
SRC_DIR = src
LIB_DIR = lib
BIN = SiftGPU

CFLAGS =  -I$(INC_DIR) -fPIC -L/usr/lib64 -L/usr/lib -L./bin -L./lib -Wall -Wno-deprecated -pthread

SIFTGPU_SRC = SiftGPU.cpp

#librarys for SiftGPU
ifneq ($(DARWIN),) #MAC 
LIBS_SIFTGPU = -lGLEW -framework GLUT -lIL -framework OpenGL -framework cocoa -framework Foundation 
CFLAGS +=  -L/opt/local/lib -L/usr/local/lib -Linclude/
else
LIBS_SIFTGPU = -lGLEW -lglut -lIL -lGL -lX11
CFLAGS += -L/usr/lib/nvidia-current/ -L/usr/include/GL/
endif

LIBS_DRIVER = $(LIB_DIR)/libsiftgpu.a $(LIBS_SIFTGPU) 
LIBS_SIMPLESIFT = $(LIBS_DRIVER) -DSIFTGPU_STATIC

KEYMATCHFULL=KeyMatchFull

all: sift
	 	
%.o : %.cpp
	$(CXX) -c -o $@ $(CPPFLAGS) $(WXFLAGS) $(BUNDLER_DEFINES) $<


sift:
	$(CC) -o $(BIN) $(SRC_DIR)/$(SIFTGPU_SRC) -Llib/ -ldl $(LIBS_SIMPLESIFT) $(CFLAGS)-static-libgcc 

test:
	$(CC) -o test $(SRC_DIR)/test.cpp -Llib/ -ldl $(LIBS_SIMPLESIFT) $(CFLAGS)

$(KEYMATCHFULL): $(SRC_DIR)/KeyMatchFull/KeyMatchFull.o $(SRC_DIR)/KeyMatchFull/keys2a.o
	$(CXX) -o $@ $(CPPFLAGS) $(LIB_PATH) $(SRC_DIR)/KeyMatchFull/KeyMatchFull.o $(SRC_DIR)/KeyMatchFull/keys2a.o \
		-lANN_char -lz
clean:
	rm -f $(BIN)
