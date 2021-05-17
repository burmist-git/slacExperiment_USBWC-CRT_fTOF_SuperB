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

CXX	      += -I./obj/
OUTLIB	      = ./obj/
.SUFFIXES: .cc,.C
.PREFIXES: ./obj/

#----------------------------------------------------#

runApp: run.cpp obj/wfSim.o obj/waveform.o
	$(CXX) $(CXXFLAGS) -o runApp $(OUTLIB)/*.o $(GLIBS) $<

obj/wfSim.o: src/wfSim.cpp src/wfSim.hh src/waveform.cpp src/waveform.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)wfSim.o $<

obj/waveform.o: src/waveform.cpp src/waveform.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)waveform.o $<

clean:
	rm -f *~
	rm -f src/*~
	rm -f Histos/*~
	rm -f $(OUTLIB)*.o
	rm -f runApp
