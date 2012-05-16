all: main

# which compiler?
#CC = /opt/mpi/bin/mpiCC
#CC = g++

CC = /usr/pgi/linux86/5.1/bin/pgCC
RUN = /usr/pgi/linux86/5.1/bin/mpirun

#RUN = /opt/mpi/bin/mpirun

FLAGS = -lmpich -Mmpi

# '@' - nevypise prikaz ale iba jeho vysledok, prepinac '-e' spracuvava take vyrazy ako \n a pod.

main: main.o procesor.o farba.o funkcie.o binswap.o kdtree.o vektor3.o
	$(CC) -o main main.o procesor.o farba.o funkcie.o binswap.o kdtree.o vektor3.o $(FLAGS)
	@echo -e "\nProgram uspesne vytvoreny!"

main.o: main.cpp
	$(CC) -c main.cpp

farba.o: farba.cpp farba.h definicie.h funkcie.h
	$(CC) -c farba.cpp

procesor.o: procesor.cpp procesor.h
	$(CC) -c procesor.cpp

funkcie.o: funkcie.cpp funkcie.h vektor3.h
	$(CC) -c funkcie.cpp

binswap.o: binswap.cpp binswap.h kdtree.h vektor3.h definicie.h
	$(CC) -c binswap.cpp

kdtree.o: kdtree.cpp kdtree.h vektor3.h
	$(CC) -c kdtree.cpp

vektor3.o: vektor3.cpp vektor3.h
	$(CC) -c vektor3.cpp


# clean up
# - pred rm znamena, ze make ma ignorovat pripadne chyby

clean:
	-rm main.o farba.o procesor.o funkcie.o binswap.o kdtree.o vektor3.o farba.cpp~ farba.h~ procesor.cpp~ procesor.h~ funkcie.cpp~ funkcie.h~ binswap.cpp~ binswap.h~ kdtree.cpp~ vektor3.cpp~ vektor3.h~ Makefile~ kdtree.h~ definicie.h~ poznamky.txt~ main.cpp~

# backup. zbali vsetky subory v akt. dir do tar.gz
backup:
	rm -rf backup.tar.gz
	tar cfz backup.tar.gz *

# builds and run on two cluster nodes
run: all
	clear
	$(RUN) -np 2 ./main

# builds and run on four cluster nodes	
runmore: all
	clear
	$(RUN) -np 4 ./main

