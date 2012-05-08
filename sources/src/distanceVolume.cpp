// distanceVolume.cpp 3D
// by skip 2006
//
// Computes distance volume in 3d matrix
// dv_compute3d - pozor na spravne aplikovanie sablony odhora dole

#include "../include/distanceVolume.h"


void dv_init3x3x3(unsigned short* in, short* out, int x, int y, int z)
{
	int okraj, mimo;

	for (int k=0; k<z; k++) 
		for (int j=y-1; j>=0; j--)
			for (int i=0; i<x; i++) {

				// ak voxel mimo objektu
				if (in[dv_getIndex(i,j,k, x,y)]==0) {
					okraj=5;
					mimo=dv_INFINITY;
				} else
				{
					// voxel vo vnutri objektu
					okraj=-5;
					mimo=-dv_INFINITY;
				}
					
				// ak susedi so stenou objektu tak automaticky vzdialenost +-0.5
						 if ( (i<x-1) && (in[dv_getIndex(i+1,j,k, x,y)]!=0) ) out[dv_getIndex(i,j,k, x,y)]=okraj;
				else if ( (i>0)   && (in[dv_getIndex(i-1,j,k, x,y)]!=0) ) out[dv_getIndex(i,j,k, x,y)]=okraj;

				else if ( (j<y-1) && (in[dv_getIndex(i,j+1,k, x,y)]!=0) ) out[dv_getIndex(i,j,k, x,y)]=okraj;
				else if ( (j>0)   && (in[dv_getIndex(i,j-1,k, x,y)]!=0) ) out[dv_getIndex(i,j,k, x,y)]=okraj;

				else if ( (k<z-1) && (in[dv_getIndex(i,j,k+1, x,y)]!=0) ) out[dv_getIndex(i,j,k, x,y)]=okraj;
				else if ( (k>0)   && (in[dv_getIndex(i,j,k-1, x,y)]!=0) ) out[dv_getIndex(i,j,k, x,y)]=okraj;

				else out[dv_getIndex(i,j,k, x,y)]=mimo;
					
			}
}



int dv_getIndex(int i, int j, int k, int sizex, int sizey)
{
	return i + j*(sizex) + k*(sizex*sizey);
}



void dv_compute3d(short* p, int x, int y, int z)
{
	// template
	
	// rozmery
	int tx = 3, ty = 3, tz = 3;
	// data
	int upT[3][3][3];
	int downT[3][3][3];

	// inicializacia 3D templatov
	int i,j,k;
	for (k=0; k<3; k++) 
		for (j=2; j>=0; j--) 
			for (i=0; i<3; i++) {
				upT[i][j][k]=-1;
				downT[i][j][k]=-1;
			}
  /* kvoli cim vyssej rychlosti treba mat nastevne vsetky vektory chamfer metriky!!! */
  /* manhatanska je pomala */

  // nasledujuce 2 skupiny tvoria manhattansku metriku
	upT[1][1][1]=0;
	upT[1][2][1]=10; // 0,1,0
	upT[0][1][1]=10; // 1,0,0
	upT[1][1][0]=10; // 0,0,1

	downT[1][1][1]=0;
	downT[1][0][1]=10; // 0,-1,0
	downT[2][1][1]=10; // -1,0,0
	downT[1][1][2]=10; // 0,0,-1
	
	// tieto dalsie spresnuju manhattansku metriku na tzv chamfer metrics
	upT[0][2][0]=17;
	upT[2][2][0]=17;
	upT[1][2][0]=14;
	upT[0][1][0]=14;
	upT[2][1][0]=14;

	upT[0][2][1]=14;
	upT[2][2][1]=14;

	upT[1][2][2]=14;
	upT[0][2][2]=17;
	upT[2][2][2]=17;

	downT[2][0][2]=17;
  downT[0][0][2]=17;
	downT[1][0][2]=14;
	downT[0][1][2]=14;
	downT[2][1][2]=14;

	downT[0][0][1]=14;
	downT[2][0][1]=14;

	downT[1][0][0]=14;
	downT[0][0][0]=17;
	downT[2][0][0]=17;



	int min, max; // minimalna/maximalna zo sucov na prilozenej maske/template
	int tmp; // ak je pozicia mimo rozmerov pola tak vratime ako keby na tej pozicii bolo nekonecno

	
	// 1st chamfering
	for (k=0; k<z; k++)
//	for (k=z-1; k>=z; k--)
		for (j=y-1; j>=0; j--) // traverzovanie musi byt zhora dole (1. vlna); (0,0,0) je lavo dole vpredu
			for (i=0; i<x; i++) {
				// ak je to objekt alebo voxel pri objekte tak nic nemenime
				if ( (p[dv_getIndex(i,j,k, x,y)]!=5) && (p[dv_getIndex(i,j,k, x,y)]!=-5) ) {

					// minimalna hodnota na nekonecno
					min = dv_INFINITY;
					max = -dv_INFINITY;

					// prechadzame pixely templatu <> -1 
					for (tz=-1; tz<2; tz++)
						for (ty=-1; ty<2; ty++)
							for (tx=-1; tx<2; tx++) {
								if (upT[tx+1][ty+1][tz+1]!=-1) {

									// podla znamienka urcime what to do
									if (p[dv_getIndex(i,j,k, x,y)]>0) {

										// osetrenie okrajovych pripadov
										if ( (i+tx<0) || (i+tx>=x) || (j+ty<0) || (j+ty>=y) || (k+tz<0) || (k+tz>=z) ) 
											tmp = dv_INFINITY; else tmp = p[dv_getIndex(i+tx,j+ty,k+tz, x,y)];
										// ulozime si najmensi sucet v maske
										if (min>upT[tx+1][ty+1][tz+1]+tmp) min=upT[tx+1][ty+1][tz+1]+tmp;

									} else {

										if ( (i+tx<0) || (i+tx>=x) || (j+ty<0) || (j+ty>=y) || (k+tz<0) || (k+tz>=z) ) 
											tmp = -dv_INFINITY; else tmp = p[dv_getIndex(i+tx,j+ty,k+tz, x,y)];
										if (max<tmp-upT[tx+1][ty+1][tz+1]) max=tmp-upT[tx+1][ty+1][tz+1];

									}

								}
							}
					// nastavenie novej vzdialenosti k objektu
					if (p[dv_getIndex(i,j,k, x,y)]>0) p[dv_getIndex(i,j,k, x,y)]=min; else p[dv_getIndex(i,j,k, x,y)]=max;
				}
			}

	// 2nd chamfering
	for (k=z-1; k>=0; k--)
//	for (k=0; k<z; k++)
		for (j=0; j<y; j++)
			for (i=x-1; i>=0; i--) {
				// ak je to objekt alebo voxel pri objekte tak nic nemenime
				if ( (p[dv_getIndex(i,j,k, x,y)]!=5) && (p[dv_getIndex(i,j,k, x,y)]!=-5) ) {
					
					// minimalna hodnota na nekonecno
					min = dv_INFINITY;
					max = -dv_INFINITY;

					// prechadzame pixely templatu <> -1 
					for (tz=-1; tz<2; tz++)
						for (ty=-1; ty<2; ty++)
							for (tx=-1; tx<2; tx++) {
								if (downT[tx+1][ty+1][tz+1]!=-1) {

									// podla znamienka urcime what to do
									if (p[dv_getIndex(i,j,k, x,y)]>0) {

										// osetrenie okrajovych pripadov
										if ( (i+tx<0) || (i+tx>=x) || (j+ty<0) || (j+ty>=y) || (k+tz<0) || (k+tz>=z) )
											tmp = dv_INFINITY; else tmp = p[dv_getIndex(i+tx,j+ty,k+tz, x,y)];
										// ulozime si najmensi sucet v maske
										if (min>downT[tx+1][ty+1][tz+1]+tmp) min=downT[tx+1][ty+1][tz+1]+tmp;

									} else {

										if ( (i+tx<0) || (i+tx>=x) || (j+ty<0) || (j+ty>=y) || (k+tz<0) || (k+tz>=z) )
											tmp = -dv_INFINITY; else tmp = p[dv_getIndex(i+tx,j+ty,k+tz, x,y)];
										if (max<tmp-downT[tx+1][ty+1][tz+1]) max=tmp-downT[tx+1][ty+1][tz+1];

									}

								}
							}
					// nastavenie novej vzdialenosti k objektu
					if (p[dv_getIndex(i,j,k, x,y)]>0) p[dv_getIndex(i,j,k, x,y)]=min; else p[dv_getIndex(i,j,k, x,y)]=max;
				}
			}
}



void dv_celldist3d(short* dv, int x, int y, int z, unsigned char *p) // vstup: 2d pole dv vzdialenosti voxelov, jeho rozmery
{
	int min; // minimum/avg zo vzdialenosti 2x2x2 bunky

	// prechadzame vsetky bunky 3d pola
	for (int k=0; k<z-1; k++)
		for (int j=0; j<y-1; j++)
			for (int i=0; i<x-1; i++) {
				
				// ak je v bunke aspon jeden objekt (zaporne cislo) tak 0
				if ( (dv[dv_getIndex(i, j,   k,   x,y)]<0) || (dv[dv_getIndex(i+1, j  , k  , x,y)]<0) ||
					   (dv[dv_getIndex(i, j+1, k,   x,y)]<0) || (dv[dv_getIndex(i+1, j+1, k  , x,y)]<0) ||
						 (dv[dv_getIndex(i, j,   k+1, x,y)]<0) || (dv[dv_getIndex(i+1, j  , k+1, x,y)]<0) ||
						 (dv[dv_getIndex(i, j+1, k+1, x,y)]<0) || (dv[dv_getIndex(i+1, j+1, k+1, x,y)]<0) ) p[dv_getIndex(i,j,k, x-1,y-1)]=0;
				else {
							// hladame minimalnu vzdialenost; avg dava lepsiu aproximaciu (menej chybnych vzd)
							//min=dv_INFINITY; // minimal vzd
							min=0; // avg vzd

							// prechadzame bunku 2x2x2
							for (int tz=0; tz<2; tz++)
								for (int ty=0; ty<2; ty++)
									for (int tx=0; tx<2; tx++) {
										//if (min>dv[dv_getIndex(i+tx, j+ty, k+tz, x,y)]) min=dv[dv_getIndex(i+tx, j+ty, k+tz, x,y)];
										min+=dv[dv_getIndex(i+tx, j+ty, k+tz, x,y)];
									}

							// ulozene hodnoty reprezentuju real, konvertujeme ich na integer. vysledok je o 1 vacsi
							//min=(min/10)+1;
							min=min/80; 
							if (min>255) min=255; // vysledne pole udrzi maximalne 8bitovu vzialenost

							p[dv_getIndex(i,j,k, x-1,y-1)]=min;

				}			
			}
}
