# C++ headers
IDIR =../include

# which compiler?
# -L/usr/X11R6/lib
CC = g++
OPT = -Wall -march=athlon-xp -O3 -pipe -fomit-frame-pointer -fmerge-all-constants -ffast-math -mmmx -msse 
# -msse2 -fssa moj athlon nepodporuje
CFLAGS=-I$(IDIR) -Wall $(OPT)
LDFLAGS = -L/usr/X11R6/lib $(OPT)


# directory where to create object files
ODIR=obj
LDIR =../lib

# include math lib
LIBS=-lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

# headers
_DEPS = raycaster.h kamera.h data.h distanceVolume.h farba.h funkcie.h svetlo.h shader.h bezier.h matica.h loadlsm.h definicie.h kvader.h ray.h timer.h vektor3.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = rc_linux.o raycaster.o kamera.o data.o distanceVolume.o farba.o funkcie.o svetlo.o shader.o bezier.o matica.o loadlsm.o kvader.o ray.o timer.o vektor3.o
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

# backup all files in current directory into the tar.gz
backup:
	rm -rf rc-linux-backup.tar.gz
	tar cfz rc-linux-backup.tar.gz ../*

# build and run
run: all
	clear
	./main
