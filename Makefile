
# How to build instructions
#
# (1) Runnable version without need to install (assets are searched in current directory)
#
# make clean
# make
# ./blackvoxel
#
# (2) Installable version with standard paths
#
# make clean
# make installable
# make install doinstall=true
#
# (3) Installable version with customized installation paths
#
# make clean
# make BV_DATA_LOCATION_DIR=/usr/local/share/blackvoxel
# make install doinstall=true BV_DATA_INSTALL_DIR=/usr/local/share/blackvoxel BV_BINARY_INSTALL_DIR=/usr/local/games


ifndef BV_DATA_LOCATION_DIR
  BV_DATA_LOCATION_DIR=""
endif

ifndef PREFIX
PREFIX=/usr/local
endif

ifndef DATAROOTDIR
DATAROOTDIR=$(DESTDIR)$(PREFIX)/share
endif

ifndef BV_BINARY_INSTALL_DIR
BV_BINARY_INSTALL_DIR=$(DESTDIR)$(PREFIX)/games
else 
override BV_BINARY_INSTALL_DIR:=$(DESTDIR)$(BV_BINARY_INSTALL_DIR)
endif

ifndef BV_DATA_INSTALL_DIR
BV_DATA_INSTALL_DIR=$(DATAROOTDIR)/blackvoxel
else
override BV_DATA_INSTALL_DIR:="$(DESTDIR)$(BV_DATA_INSTALL_DIR)"
endif

# Base options
CXX?=g++
PROGNAME=blackvoxel
CXXFLAGS+=-I "src/sc_Squirrel3/include"  -DCOMPILEOPTION_DEMO=0 -DDEVELOPPEMENT_ON=0 -DCOMPILEOPTION_SPECIAL=0 -DCOMPILEOPTION_DATAFILESPATH="\"$(BV_DATA_LOCATION_DIR)\""
SRC= $(wildcard src/*.cpp) $(wildcard src/z/*.cpp)
OBJ= $(SRC:src/%.cpp=obj/%.o)

# Operating system and architecture detection

ifeq ($(OS),Windows_NT)
  CPU_BITS=32
  CXXFLAGS+= -O3 -c -fmessage-length=0 -march=i686
  LDFLAGS+= -s -Xlinker --large-address-aware -mwindows -L"src/sc_Squirrel3/lib" -lmingw32 -lSDLmain -lSDL -llibglew32 -lglu32 -lopengl32 -llibglut -lsquirrel -lsqstdlib
else
  # Unix like operating systems
  CPU_BITS= $(shell getconf LONG_BIT)
  CPU_ARCH= $(shell uname -p)
  KERNELNAME =$(shell uname -s)

  ifeq ($(KERNELNAME),Linux)
    CXXFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS+=-s -zrelro -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  else ifeq ($(KERNELNAME), FreeBSD)
    # To be done...
    CXXFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS+=-s -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  else ifeq ($(KERNELNAME), Darwin)
    CXXFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS+=-s -L"src/sc_Squirrel3/lib" -L"/usr/local/Cellar/glew" -L"/usr/local/Cellar/sdl" -I"/usr/local/Cellar/glew" -I"/usr/local/Cellar/sdl" -framework Cocoa -framework OpenGL -lSDLmain -lSDL -lGLEW -lsquirrel -lsqstdlib
  else
    # Unknow kernel... trying default flags
    CXXFLAGS+= -O3 -c -fmessage-length=0
    LDFLAGS+=-s -L"src/sc_Squirrel3/lib" -lGLU -lSDL -lGLEW -lGL -lsquirrel -lsqstdlib
  endif
endif


obj/%.o: src/%.cpp
	@mkdir -p obj/z
	$(CXX) -o $@ -c $< $(CXXFLAGS) 

all: $(PROGNAME)

$(PROGNAME): $(OBJ) squirrel
	$(CXX) -o $(PROGNAME) $(OBJ) $(LDFLAGS)

installable: BV_DATA_LOCATION_DIR=$(BV_DATA_INSTALL_DIR)
installable: all

squirrel: 
	cd src/sc_Squirrel3 ; $(MAKE) sq$(CPU_BITS)

clean:
	@rm -rf obj
	@cd src/sc_Squirrel3 ; $(MAKE) clean
	@rm -f $(PROGNAME)

mrproper: clean
	rm -rf $(PROGNAME)

install:
ifndef doinstall
	@echo ""
	@echo ""
	@echo "************************************** INFO *****************************************"
	@echo ""
	@echo "With the default build target, Blackvoxel doesn't need to be installed. "
	@echo "The executable is set to search for it's assets in the current directory."
	@echo "So, simply run it with ./blackvoxel"
	@echo ""
	@echo "If you want to build blackvoxel for some regular installation, use the following commands"
	@echo ""
	@echo "make clean"
	@echo "make installable"
	@echo "make install doinstall=true"
	@echo ""
	@echo "(Default installation directory are : $(BV_BINARY_INSTALL_DIR) (executable) and $(BV_DATA_INSTALL_DIR)(Assets)"
	@echo ""
	@echo "Installation directory for binary and assets can be customized even more following this example:"
	@echo ""
	@echo "make clean"
	@echo "make BV_DATA_LOCATION_DIR=/usr/local/share/blackvoxel"
	@echo "make install doinstall=true BV_DATA_INSTALL_DIR=/usr/local/share/blackvoxel BV_BINARY_INSTALL_DIR=/usr/local/games"
	@echo ""
	@echo ""
	

else
	mkdir -p $(BV_BINARY_INSTALL_DIR)
	cp blackvoxel $(BV_BINARY_INSTALL_DIR)
	chmod u=rwx,g=rx,o=rx $(BV_BINARY_INSTALL_DIR)/blackvoxel
	mkdir -p $(BV_DATA_INSTALL_DIR)
	cp -r gui        $(BV_DATA_INSTALL_DIR)
	cp -r Misc       $(BV_DATA_INSTALL_DIR)
	cp -r Sound      $(BV_DATA_INSTALL_DIR)
	cp -r VoxelTypes $(BV_DATA_INSTALL_DIR)
	cp randomnum.dat $(BV_DATA_INSTALL_DIR)
	cp Copyright.txt $(BV_DATA_INSTALL_DIR)
	cp 2>/dev/null   LICENSE       $(BV_DATA_INSTALL_DIR) | true
	cp 2>/dev/null   Licence.txt   $(BV_DATA_INSTALL_DIR) | true
	cp 2>/dev/null   Licence_3rdParty_libs.txt $(BV_DATA_INSTALL_DIR) | true
	chmod -R u=rwX,g=rX,o=rX $(BV_DATA_INSTALL_DIR)
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
