#ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
#ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
#ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

ROOTCFLAGS    = $(shell /usr/bin/root-config --cflags)
ROOTLIBS      = $(shell /usr/bin/root-config --libs)
ROOTGLIBS     = $(shell /usr/bin/root-config --glibs)

CXX           = g++
CXXFLAGS      = -g -Wall -fPIC -Wno-deprecated

NGLIBS         = $(ROOTGLIBS) 
NGLIBS        += -lMinuit

CXXFLAGS      += $(ROOTCFLAGS)
CXX           += -I./	
LIBS           = $(ROOTLIBS) 

GLIBS          = $(filter-out -lNew, $(NGLIBS))

CXX	      += -I./lib/
OUTLIB	      = ./lib/
.SUFFIXES: .C
.PREFIXES: ./lib/

#----------------------------------------------------#

all: hist_add.C
	$(CXX) $(CXXFLAGS) -o hist_add  $(GLIBS) $<


clean:
	rm -f hist_add
	rm -f *~
