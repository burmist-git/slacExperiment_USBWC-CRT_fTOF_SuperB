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

runApp: run.cpp obj/slacTestAna.o obj/wfSim.o obj/waveform.o
	$(CXX) $(CXXFLAGS) -o runApp $(OUTLIB)/*.o $(GLIBS) $<

obj/slacTestAna.o: src/slacTestANA.cpp src/slacTestANA.hh ../anaConst.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)slacTestAna.o $<

obj/wfSim.o: ../wfSimulation/src/wfSim.cpp ../wfSimulation/src/wfSim.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)wfSim.o $<

obj/waveform.o: ../wfSimulation/src/waveform.cpp ../wfSimulation/src/waveform.hh
	$(CXX) $(CXXFLAGS) -c -I. -o $(OUTLIB)waveform.o $<

clean:
	rm -f *~
	rm -f src/*~
	rm -f Histos/*~
	rm -f $(OUTLIB)*.o
	rm -f runApp
