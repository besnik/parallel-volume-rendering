# headers
IDIR =../include

# which compiler?
# -L/usr/X11R6/lib
CC = /opt/mpi/bin/mpiCC
RUN = /opt/mpi/bin/mpirun

#CC = /usr/pgi/linux86/5.1/bin/pgCC
#RUN = /usr/pgi/linux86/5.1/bin/mpirun

#CC = /usr/local/mpich-1.2.7/bin/mpiCC
#RUN = /usr/local/mpich-1.2.7/bin/mpirun

# g++
#OPT = -Wall
OPT = -Wall -march=pentium4 -O3 -pipe -fomit-frame-pointer -fmerge-all-constants -ffast-math -mmmx -msse -msse2 -fssa
#OPT = -Wall -march=pentium4 -O3 -pipe -fomit-frame-pointer

# pgCC
#OPT =
#OPT = -O3 -s -fast
#OPT = -Mcache_align -tp p7 -fastsse -O3 -fastsse -Minline -Mvect=sse -s
# nepustat ! soo slow OPT = -O3 -tp piv -fastsse -Minline -Mvect=sse -Mconcur -Mipa=fast -pc 32 -Mprof

ROPT = -v 

CFLAGS=-I$(IDIR) $(OPT)
LDFLAGS = $(OPT)


# directory with objects
ODIR=obj
LDIR =../lib

# include math lib
LIBS=-lm -lmpich

# here we have headers 
_DEPS = procesor.h raycaster.h shader.h svetlo.h farba.h bezier.h funkcie.h binswap.h init.h kdtree.h distanceVolume.h data.h kvader.h ray.h kamera.h matica.h vektor3.h loadlsm.h timer.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o procesor.o raycaster.o shader.o svetlo.o farba.o bezier.o funkcie.o binswap.o init.o kdtree.o distanceVolume.o data.o kvader.o ray.o kamera.o matica.o vektor3.o loadlsm.o timer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# '@' - nevypise prikaz ale iba jeho vysledok, prepinac '-e' spracuvava take vyrazy ako \n a pod.

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)
	@echo -e "\nProgram uspesne vytvoreny!"

# clean up
# - pred rm znamena, ze make ma ignorovat pripadne chyby
# .PHONY rule keeps make from doing something with a file named clean.

.PHONY: clean

clean:
	-rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

# backup. zbali vsetky subory v akt. dir do tar.gz
backup:
	rm -rf klaster3-backup.tar.gz
	tar cfz klaster3-backup.tar.gz ../*

# builds and execute. on how many nodes in the farm it shall execute?
run: main
	$(RUN) $(ROPT) -np 2 ./main
	
runmore: main
	$(RUN) $(ROPT) -np 4 ./main

run2: main
	$(RUN) $(ROPT) -np 2 ./main

run4: main
	$(RUN) $(ROPT) -np 4 ./main

run8: main
	$(RUN) $(ROPT) -np 8 ./main

run16: main
	$(RUN) $(ROPT) -np 16 ./main

