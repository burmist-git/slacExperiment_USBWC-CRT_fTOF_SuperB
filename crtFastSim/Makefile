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

App: crtFastSim.cpp crtDetector.o muonGen.o
	$(CXX) $(CXXFLAGS) -o crtApp obj/*.o $(GLIBS) $<

muonGen.o: src/muonGen.cpp src/muonGen.hh crtTrk.o
	$(CXX) $(CXXFLAGS) -c -I. -o obj/muonGen.o $<

crtDetector.o: src/crtDetector.cpp src/crtDetector.hh crtBox.o crtTrk.o
	$(CXX) $(CXXFLAGS) -c -I. -o obj/crtDetector.o $<

crtBox.o: src/crtBox.cpp src/crtBox.hh src/crtTrk.cpp src/crtTrk.hh src/crtConst.hh crtTrk.o plane.o
	$(CXX) $(CXXFLAGS) -c -I. -o obj/crtBox.o $<

crtTrk.o: src/crtTrk.cpp src/crtTrk.hh src/crtConst.hh
	$(CXX) $(CXXFLAGS) -c -I. -o obj/crtTrk.o $<

plane.o:  src/plane.cpp src/plane.hh
	$(CXX) $(CXXFLAGS) -c -I. -o obj/plane.o $<

clean:
	rm -f crtApp
	rm -f obj/*.o
	rm -f *~ src/*~