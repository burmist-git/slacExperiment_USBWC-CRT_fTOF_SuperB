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

all:  mergeDataFilesUSB convertCRT2root convertUSBWC2root

mergeDataFilesUSB: mergeDataFilesUSB.cc
	$(CXX) $(CXXFLAGS) -o mergeDataFilesUSBApp  $(GLIBS) $<

convertCRT2root: convertCRT2root.cc
	$(CXX) $(CXXFLAGS) -o convertCRT2rootApp  $(GLIBS) $<

convertUSBWC2root: convertUSBWC2root.cc
	$(CXX) $(CXXFLAGS) -o convertUSBWC2rootApp  $(GLIBS) $<

clean:
	rm -f mergeDataFilesUSBApp
	rm -f convertCRT2rootApp
	rm -f convertUSBWC2rootApp
	rm -f obj/*.o
	rm -f *~ src/*~
