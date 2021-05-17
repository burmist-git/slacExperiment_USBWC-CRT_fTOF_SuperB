# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := fTOF
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

CPPFLAGS += -I$(ROOTSYS)/include/root
#CPPFLAGS += -I/usr/include/root

EXTRALIBS = $(shell root-config --glibs)
#EXTRALIBS += -L$(ROOTSYS)/lib -lTree -lRIO -lNet -lMathCore -lCore -lCint -lMathCore


# Add GNU Scientific Library
CPPFLAGS += -I/usr/include/gsl/
EXTRALIBS += -L$(shell gsl-config --libs)

include $(G4INSTALL)/config/binmake.gmk
