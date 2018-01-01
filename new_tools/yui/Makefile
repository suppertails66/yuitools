IDIR := src
SRCDIR := src
ODIR := obj
LDIR :=

BLACKTDIR = ../blackt
LIBMDDIR = ../libmd

AR=ar
CXX=g++
# Compile only
CDEFINES = -DBLACKT_ENABLE_LIBPNG
#CLIBS = -lpng
CFLAGS = -std=gnu++11 -O2 -Wall -L${LIBMDDIR} -lmd -L${BLACKTDIR} -lblackt -lpng
CINCLUDES = -I${BLACKTDIR}/src -I${LIBMDDIR}/src
CXXFLAGS=$(CFLAGS) $(CDEFINES) $(CINCLUDES) -I$(IDIR)

#SRC := $(wildcard $(SRCDIR)/*/*.cpp)
#OBJ := $(patsubst $(SRCDIR)/%,$(ODIR)/%,$(patsubst %.cpp,%.o,$(SRC)))
#DEP := $(patsubst %.o,%.d,$(OBJ))
#LIB := libmd.a

LIBDEPS := $(LIBMDDIR)/libmd.a $(BLACKTDIR)/libblackt.a

TOOLSRCS := $(wildcard $(SRCDIR)/*.cpp)
#TOOLS = test opbextr opbinsr opbfileextr opbfileinsr dsimgconv
TOOLSINDIR := $(patsubst %.cpp,%,$(TOOLSRCS))
TOOLS := $(notdir $(TOOLSINDIR))

all: blackt libmd $(TOOLS)

blackt:
	cd ${BLACKTDIR} && $(MAKE) && cd $(CURDIR)

libmd:
	cd ${LIBMDDIR} && $(MAKE) && cd $(CURDIR)
	
#$(SRCDIR)/%: blackt libmd
#	$(CXX) $@.cpp $(OBJ) -o $(notdir $@) $(CXXFLAGS)

grpdmp: $(SRCDIR)/grpdmp.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

srecpatch: $(SRCDIR)/srecpatch.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

datinjct: $(SRCDIR)/datinjct.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

datpatch: $(SRCDIR)/datpatch.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

romprep: $(SRCDIR)/romprep.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

golfer_fontrip: $(SRCDIR)/golfer_fontrip.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

golfer_fontbuild: $(SRCDIR)/golfer_fontbuild.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

golfer_rle4decmp: $(SRCDIR)/golfer_rle4decmp.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

golfer_rle4cmp: $(SRCDIR)/golfer_rle4cmp.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

golfer_tilemaprip: $(SRCDIR)/golfer_tilemaprip.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

grpunmap: $(SRCDIR)/grpunmap.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

tilemapper: $(SRCDIR)/tilemapper.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

golfer_tilemapbuild: $(SRCDIR)/golfer_tilemapbuild.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

tiletext_dump: $(SRCDIR)/tiletext_dump.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

tiledmp: $(SRCDIR)/tiledmp.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

tileundmp: $(SRCDIR)/tileundmp.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

introrip: $(SRCDIR)/introrip.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

maptext_dump: $(SRCDIR)/maptext_dump.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

scriptptr_dump: $(SRCDIR)/scriptptr_dump.cpp $(LIBDEPS)
	$(CXX) $(SRCDIR)/$@.cpp $(OBJ) -o $@ $(CXXFLAGS)

.PHONY: blackt libmd cleanme clean

cleanme:
	rm -f $(TOOLS)

clean: cleanme
#	rm -f $(LIB)
#	rm -rf $(ODIR)
	cd ${BLACKTDIR} && $(MAKE) clean && cd $(CURDIR)
	cd ${LIBMDDIR} && $(MAKE) clean && cd $(CURDIR)