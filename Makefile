CC = gcc
CXX = g++
CCFLAGS = -g -O1 -W -Wall -Wno-unused-parameter -Wno-ignored-qualifiers #-pedantic -fPIC
ROOTFLAGS = `root-config --cflags --glibs`
ROOTVERSION = -D ROOT5
LHAPDF = /sps/compass/npierre/LHAPDF6
LHAPDF_INCL += -I$(LHAPDF)/include
LHAPDF_LIBS += -L$(LHAPDF)/lib -lLHAPDF

ifeq ($(DEBUG),1)
CCFLAGS += -DDEBUG
endif

all : plot_dis

%.o: %.cc %.h
	@$(CXX) $(CCFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -c -o $@ $<

plot_dis: plot_dis.cc plot_dis.h HistLoader.h GetFlux.h
	@echo 'Building DVCS analysis package..'
	@$(CXX) $(CCFLAGS) -Wno-ignored-qualifiers $(ROOTFLAGS) $(ROOTVERSION) -o $@ $<

clean :
	@rm -rf *.o DVCS_analysis
