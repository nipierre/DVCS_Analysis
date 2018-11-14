CC = gcc
CXX = g++
CCFLAGS = -g -O1 -W -Wall -Wno-unused-parameter -Wno-ignored-qualifiers #-pedantic -fPIC
ROOTFLAGS = `root-config --cflags --glibs`
ROOTVERSION = -D ROOT5
LFLAGS = -I./include
LHAPDF = /sps/compass/npierre/LHAPDF6
LHAPDF_INCL += -I$(LHAPDF)/include
LHAPDF_LIBS += -L$(LHAPDF)/lib -lLHAPDF

ifeq ($(DEBUG),1)
CCFLAGS += -DDEBUG
endif

all : plot_dis

GetFlux.o: GetFlux.cc include/GetFlux.h
	$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -c -o $@ $<

HistLoader.o: HistLoader.cc include/HistLoader.h
	$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -c -o $@ $<

plot_dis.o: plot_dis.cc include/plot_dis.h include/GetFlux.h include/HistLoader.h
	$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -c -o $@ $<

plot_dis: plot_dis.o GetFlux.o HistLoader.o
	@echo 'Building DVCS analysis package..'
	$(CXX) $(CCFLAGS) -Wno-ignored-qualifiers $(ROOTFLAGS) $(ROOTVERSION) -o $@ $< GetFlux.o HistLoader.o

clean :
	@rm -rf *.o plot_dis
