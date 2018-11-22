CC = gcc
CXX = g++
ROOTFLAGS = `root-config --cflags --glibs`
CCFLAGS = -g -O1 -W -Wall -Wno-unused-parameter -Wno-ignored-qualifiers -Wno-unused-function -Wl,--no-as-needed $(ROOTFLAGS)#-pedantic -fPIC
# I had to add -Wl,--no-as-needed
LFLAGS = -I./include
OBJSF = plot_dis.o GetFlux.o HistLoader.o

ifeq ($(DEBUG),1)
CCFLAGS += -DDEBUG
endif

all : plot_dis plot_target
		@echo 'DVCS analysis package built !'

GetFlux.o: GetFlux.cc include/GetFlux.h
	@$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) -c -o $@ $<

HistLoader.o: HistLoader.cc include/HistLoader.h
	@$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) -c -o $@ $<

plot_dis.o: plot_dis.cc include/plot_dis.h include/GetFlux.h include/HistLoader.h
	@echo 'Building DVCS analysis package..'
	@$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) -c -o $@ $<

plot_dis: plot_dis.o GetFlux.o HistLoader.o
	@$(CXX) $(CCFLAGS) -Wno-ignored-qualifiers $(ROOTFLAGS) -o $@ $(OBJSF)
	@echo 'plot_dis built !'


target_coordinates.o: TargetCoordinates.cc include/TargetCoordinates.h
	@$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) -c -o $@ $<

plot_target.o: plot_target.cc include/plot_target.h include/TargetCoordinates.h include/HistLoader.h
	@$(CXX) $(CCFLAGS) $(LFLAGS) $(ROOTFLAGS) -c -o $@ $<

plot_target: plot_target.o target_coordinates.o HistLoader.o
	@$(CXX) $(CCFLAGS) -Wno-ignored-qualifiers $(ROOTFLAGS) -o $@ $^
	@echo 'plot_target built !'

clean :
	@rm -rf *.o plot_dis plot_target
