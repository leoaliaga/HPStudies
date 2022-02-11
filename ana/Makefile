
OBJS_LIB = $(shell ls *.cxx | sed 's/\.cxx/.o/')

PROGS = $(shell ls *.C | cut -d'.' -f1)

INCLUDES = -I${G4WORKDIR}/include -I$(shell root-config --incdir) 
DEPLIBS  = $(shell root-config --libs) -lEG
CC	 = g++
COPTS	 = -fPIC -DLINUX -O0  -g $(shell root-config --cflags) 

all:    lib programs 

lib: libhputils.so

libhputils.so: $(OBJS_LIB)
	$(CC) -shared -o $@ $^ 

programs: $(PROGS)
	echo making $(PROGS)

$(PROGS): % : %.o $(G4WORKDIR)/build/libg4hpDict.so $(G4WORKDIR)/ana/libhputils.so
	$(CC) -Wall -o $@ $< $(DEPLIBS) -L$(G4WORKDIR)/build -L$(G4WORKDIR)/ana -lg4hpDict -lhputils

%.o: %.C
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

%.o: %.cxx
	$(CC) $(COPTS) $(INCLUDES) -c -o $@ $<

clean: 
	-rm *.o *.so $(PROGS) 

