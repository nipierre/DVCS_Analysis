CC = gcc
CXX = g++
CCFLAGS = -g -O1 -W -Wall -Wno-unused-parameter -Wno-ignored-qualifiers -Wno-unused-function #-pedantic -fPIC
ROOTFLAGS = `root-config --cflags --glibs`
LFLAGS = -I./include
OBJSF = plot_dis.o GetFlux.o HistLoader.o

ifeq ($(DEBUG),1)
CCFLAGS += -DDEBUG
endif

all : plot_dis

GetFlux.o: GetFlux.cc include/GetFlux.h
	@$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) -c -o $@ $<

HistLoader.o: HistLoader.cc include/HistLoader.h
	@$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) -c -o $@ $<

plot_dis.o: plot_dis.cc include/plot_dis.h include/GetFlux.h include/HistLoader.h
	@echo 'Building DVCS analysis package..'
	@$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) -c -o $@ $<

plot_dis: plot_dis.o GetFlux.o HistLoader.o
	@$(CXX) $(CCFLAGS) -Wno-ignored-qualifiers $(ROOTFLAGS) -o $@ $(OBJSF)
	@echo 'DVCS analysis package built !'

clean :
	@rm -rf *.o plot_dis
