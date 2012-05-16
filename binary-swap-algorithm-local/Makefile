all: main

# which compiler to use ?
CC = g++

# '@' - don't print command but only result, '-e' processes expressions like \n etc.

main: main.o procesor.o farba.o funkcie.o binswap.o kdtree.o vektor3.o
	$(CC) -o main main.o procesor.o farba.o funkcie.o binswap.o kdtree.o vektor3.o
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
# -rm means that make shall ignore eventual errors

clean:
	-rm main.o farba.o procesor.o funkcie.o binswap.o kdtree.o vektor3.o farba.cpp~ farba.h~ procesor.cpp~ procesor.h~ funkcie.cpp~ funkcie.h~ binswap.cpp~ binswap.h~ kdtree.cpp~ vektor3.cpp~ vektor3.h~ Makefile~ kdtree.h~ definicie.h~ poznamky.txt~ main.cpp~

# backup. crate package in current directory into tar.gz
backup:
	rm -rf binswap-backup.tar.gz
	tar cfz binswap-backup.tar.gz *

# builds and executes the program
run: all
	clear
	./main
