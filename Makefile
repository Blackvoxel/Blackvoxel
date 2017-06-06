#define paths

ifndef blackvoxeldatadir
  blackvoxeldatadir=""
endif
ifndef bindir
  bindir=""
endif

# Base options
CC=g++
LD=g++
PROGNAME=blackvoxel
CFLAGS=-I "src/sc_Squirrel3/include"  -DCOMPILEOPTION_DEMO=0 -DDEVELOPPEMENT_ON=0 -DCOMPILEOPTION_SPECIAL=0 -DCOMPILEOPTION_DATAFILESPATH="\"$(blackvoxeldatadir)\""
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
    LDFLAGS=-s -zrelro -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  else ifeq ($(KERNELNAME), FreeBSD)
    # To be done...
    CFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS=-s -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  else ifeq ($(KERNELNAME), Darwin)
    CFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS=-s -L"src/sc_Squirrel3/lib" -L"/usr/local/Cellar/glew" -L"/usr/local/Cellar/sdl" -I"/usr/local/Cellar/glew" -I"/usr/local/Cellar/sdl" -framework Cocoa -framework OpenGL -lSDLmain -lSDL -lGLEW -lsquirrel -lsqstdlib
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



squirrel: 
	cd src/sc_Squirrel3 ; make sq$(CPU_BITS)

clean:
	@rm -rf obj
	@cd src/sc_Squirrel3 ; make clean
	@rm -f $(PROGNAME)

mrproper: clean
	rm -rf $(PROGNAME)

install:
ifndef specialinstall
	echo "WARNING : with default settings Blackvoxel doesn't need to";
	echo "be installed. The executable can be run directly."
	echo "If you want to install it anyway, see documentation"
	echo "for compilation flags."
else
	mkdir -p $(DESTDIR)$(bindir)
	cp blackvoxel $(DESTDIR)$(bindir)
	chmod u=rwx,g=rx,o=rx $(DESTDIR)$(bindir)blackvoxel
	mkdir -p $(DESTDIR)$(blackvoxeldatadir)
	cp -r gui        $(DESTDIR)$(blackvoxeldatadir)
	cp -r Misc       $(DESTDIR)$(blackvoxeldatadir)
	cp -r Sound      $(DESTDIR)$(blackvoxeldatadir)
	cp -r VoxelTypes $(DESTDIR)$(blackvoxeldatadir)
	cp randomnum.dat $(DESTDIR)$(blackvoxeldatadir)
	cp Copyright.txt $(DESTDIR)$(blackvoxeldatadir)
	cp Licence.txt   $(DESTDIR)$(blackvoxeldatadir)
	cp Licence_3rdParty_libs.txt $(DESTDIR)$(blackvoxeldatadir)
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)$(blackvoxeldatadir)
  ifdef icondir
	mkdir -p $(DESTDIR)$(icondir)
	cp Icons/blackvoxel-16x16.xpm $(DESTDIR)$(icondir)
	cp Icons/blackvoxel-32x32.xpm $(DESTDIR)$(icondir)
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)$(icondir)blackvoxel-16x16.xpm
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)$(icondir)blackvoxel-32x32.xpm
  endif
  ifdef icondir2
	mkdir -p $(DESTDIR)$(icondir2)hicolor/128x128/apps
	cp Icons/blackvoxel_128x128.png $(DESTDIR)$(icondir2)hicolor/128x128/apps/blackvoxel_128x128.png
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)$(icondir2)hicolor/128x128/apps/blackvoxel_128x128.png
  endif
  ifdef desktopdir
	mkdir -p $(DESTDIR)$(desktopdir)
	cp Icons/blackvoxel.desktop     $(DESTDIR)$(desktopdir)
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)$(desktopdir)/blackvoxel.desktop
  endif
endif

#debian binary package special installation.

debian_binary_package_install:
	mkdir -p $(DESTDIR)/usr/games
	cp blackvoxel $(DESTDIR)/usr/games
	chmod u=rwx,g=rx,o=rx $(DESTDIR)/usr/games/blackvoxel
	mkdir -p $(DESTDIR)/usr/share/blackvoxel
	cp -r gui        $(DESTDIR)/usr/share/blackvoxel/
	cp -r Misc       $(DESTDIR)/usr/share/blackvoxel/
	cp -r Sound      $(DESTDIR)/usr/share/blackvoxel/
	cp -r VoxelTypes $(DESTDIR)/usr/share/blackvoxel/
	cp randomnum.dat $(DESTDIR)/usr/share/blackvoxel/
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)/usr/share/blackvoxel
	mkdir -p $(DESTDIR)/usr/share/pixmaps/
	mkdir -p $(DESTDIR)/usr/share/icons/hicolor/128x128/apps
	mkdir -p $(DESTDIR)/usr/share/applications
	cp Icons/blackvoxel-16x16.xpm $(DESTDIR)/usr/share/pixmaps/
	cp Icons/blackvoxel-32x32.xpm $(DESTDIR)/usr/share/pixmaps/
	cp Icons/blackvoxel_128x128.png $(DESTDIR)/usr/share/icons/hicolor/128x128/apps/blackvoxel.png
	cp Icons/blackvoxel.desktop     $(DESTDIR)/usr/share/applications
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)/usr/share/pixmaps/blackvoxel-16x16.xpm
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)/usr/share/pixmaps/blackvoxel-32x32.xpm
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)/usr/share/icons/hicolor/128x128/apps/blackvoxel.png
	chmod -R u=rwX,g=rX,o=rX $(DESTDIR)/usr/share/applications/blackvoxel.desktop
	
.PHONY: clean mrproper squirrel install debian_binary_package_install
