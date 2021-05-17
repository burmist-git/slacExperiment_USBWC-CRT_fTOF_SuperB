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

App: mergeCRTandUSB_App.cpp crtFileStr.o usbwcFileStr.o crtBox.o crtTrk.o ../wfSimulation/src/waveform.cpp ../wfSimulation/src/waveform.hh ../anaConst.hh AppCrtTest AppUsbTest
	$(CXX) $(CXXFLAGS) -o mergeApp obj/*.o $(GLIBS) $<

AppCrtTest: crtDataTest.cpp  crtFileStr.o crtBox.o crtTrk.o
	$(CXX) $(CXXFLAGS) -o crtDataTestApp obj/*.o $(GLIBS) $<

AppUsbTest: usbDataTest.cpp  usbwcFileStr.o
	$(CXX) $(CXXFLAGS) -o usbDataTestApp obj/*.o $(GLIBS) $<

crtFileStr.o: src/crtFileStr.cpp src/crtFileStr.hh ../anaConst.hh crtBox.o
	$(CXX) $(CXXFLAGS) -c -I. -o obj/crtFileStr.o $<

usbwcFileStr.o: src/usbwcFileStr.cpp src/usbwcFileStr.hh waveform.o
	$(CXX) $(CXXFLAGS) -c -I. -o obj/usbwcFileStr.o $<

waveform.o: ../wfSimulation/src/waveform.cpp ../wfSimulation/src/waveform.hh
	$(CXX) $(CXXFLAGS) -c -I. -o obj/waveform.o $<

crtBox.o: src/crtBox.cpp src/crtBox.hh src/crtTrk.cpp src/crtTrk.hh crtTrk.o
	$(CXX) $(CXXFLAGS) -c -I. -o obj/crtBox.o $<

crtTrk.o: src/crtTrk.cpp src/crtTrk.hh ../anaConst.hh
	$(CXX) $(CXXFLAGS) -c -I. -o obj/crtTrk.o $<

clean:
	rm -f crtDataTestApp
	rm -f usbDataTestApp
	rm -f mergeApp
	rm -f obj/*.o
	rm -f *~ src/*~