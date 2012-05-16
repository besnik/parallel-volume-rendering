// distanceVolume.h
// Distance Volume (DV) structure for RADC algorithm
// by skip 2006
// version 1.1

#include <iostream>
#include <iomanip> // I/O Manipulators


using namespace std;

#define INFINITY 32767

void dv_init3x3(short** in, short** out, int x, int y); // initialize DV for template 3x3, necessary to run as first task to calculate distances
void dv_compute(short** p, int x, int y); // calculates Distance Volume
void dv_celldist(short** dv, int x, int y, unsigned char** p); // x,y=size of the dv array, calculates distance of cells from the object (cell that contains at least one voxel)


template <class T> void dv_print(T **p, int x, int y) // vypise hodnoty pola
{
	for (int j=0; j<y; j++) {
		for (int i=0; i<x; i++) {
			std::cout << setw(4) <<		(int)p[i][j] << " ";
		}
		std::cout << std::endl;
	}
}