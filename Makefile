# Base options

CC=g++
LD=g++
PROGNAME=blackvoxel
CFLAGS=-I "src/sc_Squirrel3/include"  -DCOMPILEOPTION_DEMO=0 -DDEVELOPPEMENT_ON=0 -DCOMPILEOPTION_SPECIAL=0 -DCOMPILEOPTION_DATAFILESPATH="\"\""
SRC= $(wildcard src/*.cpp) $(wildcard src/z/*.cpp)
OBJ= $(SRC:src/%.cpp=obj/%.o)

# Operating system and architecture detection

ifeq ($(OS),Windows_NT)
  CPU_BITS=32
  CFLAGS+= -O3 -c -fmessage-length=0 -march=i686
  LDFLAGS= -s -Xlinker --large-address-aware -mwindows -L"src/sc_Squirrel3/lib" -lmingw32 -lSDLmain -lSDL -llibglew32 -lglu32 -lopengl32 -llibglut -lsquirrel -lsqstdlib
else
  # Unix like operating systems
  CPU_BITS= $(shell getconf LONG_BIT)
  CPU_ARCH= $(shell uname -p)
  KERNELNAME =$(shell uname -s)

  ifeq ($(KERNELNAME),Linux)
    CFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS=-s -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  else ifeq ($(KERNELNAME), FreeBSD)
    # To be done...
    CFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS=-s -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  else ifeq ($(KERNELNAME), Darwin)
    # To be done...
    CFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS=-s -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  else
    # Unknow kernel... trying default flags
    CFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS=-s -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  endif
endif


obj/%.o: src/%.cpp
	@mkdir -p obj/z
	$(CC) -o $@ -c $< $(CFLAGS) 
	
all: $(PROGNAME)

$(PROGNAME): $(OBJ) squirrel
	$(LD) -o $(PROGNAME) $(OBJ) $(LDFLAGS)

.PHONY: clean mrproper squirrel

squirrel: 
	cd src/sc_Squirrel3 ; make sq$(CPU_BITS)

clean:
	@rm -rf obj
	@cd src/sc_Squirrel3 ; make clean

mrproper: clean
	rm -rf $(PROGNAME)
