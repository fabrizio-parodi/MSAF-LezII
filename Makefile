CC         = g++ 
CFLAGS     = --std=c++11 -g -Wall
CFLAGSROOT = `root-config --cflags`
LIBSROOT   = `root-config --glibs`

all: SimSci

myTRandom.o: myTRandom.cpp
	$(CC) $(CFLAGS) -c myTRandom.cpp      $(CFLAGSROOT)
Geometry.o: Geometry.cpp
	$(CC) $(CFLAGS) -c Geometry.cpp      $(CFLAGSROOT)
SimSci: SimSci.cpp myTRandom.o Geometry.o
	$(CC) $(CFLAGS) -o SimSci SimSci.cpp Geometry.o myTRandom.o $(CFLAGSROOT) $(LIBSROOT
SimSci_step1: SimSci_step1.cpp myTRandom.o Geometry.o
	$(CC) $(CFLAGS) -o SimSci SimSci_step1.cpp Geometry.o myTRandom.o $(CFLAGSROOT) $(LIBSROOT)
clean:
	rm *.o
