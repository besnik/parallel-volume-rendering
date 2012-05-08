// distanceVolume.h
// Distance Volume 3D (DV) structure for RADC algorithm
// version 1.2
// to speed up calculations we work with integers as with floating point numbers (float*10)
// negative values are distances from inside of the object to the surface; positive from outside to the surface of the object.

#include <iostream>
#include <iomanip> // I/O Manipulators

using namespace std;

#define dv_INFINITY 32767


int dv_getIndex(int i, int j, int k, int sizex, int sizey); // index do 1D reprezentacie 3-D pola
void dv_init3x3x3(unsigned short* in, short* out, int x, int y, int z); // inicializuje DV pre template 3x3x3, nutne pred spustenim algoritmu compute!
void dv_compute3d(short* p, int x, int y, int z); // calculates Distance Volume 3D
void dv_celldist3d(short* dv, int x, int y, int z, unsigned char *p); // calculates distances of a cell from objects

template <class T> void dv_vypis3d(T *p, int x, int y, int z) // print values to console
{
	for (int k=0; k<z; k++) {
		for (int j=y-1; j>=0; j--) {
			for (int i=0; i<x; i++) {
				std::cout << setw(4) <<		(int)p[dv_getIndex(i,j,k, x,y)] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

/*
  verzia 1.2
	 - fixed bug in distance transform functionality


*/

