// distanceVolume.cpp
// by skip 2006
//

#include "distanceVolume.h"

void dv_init3x3(short** in, short** out, int x, int y)
{
	for (int j=0; j<y; j++)
	for (int i=0; i<x; i++) {

		// if voxel is outside of the object
		if (in[i][j]==0) {

			// if voxel is next to the surface of the object then automatically increase distance +0.5
		    if ( (i<x-1) && (in[i+1][j]==1) ) out[i][j]=5;
			else if ( (i>0)   && (in[i-1][j]==1) ) out[i][j]=5;
			else if ( (j<y-1) && (in[i][j+1]==1) ) out[i][j]=5;
			else if ( (j>0)   && (in[i][j-1]==1) ) out[i][j]=5;
			else out[i][j]=INFINITY;

		} else {

			// voxel inside the object
		    if ( (i<x-1) && (in[i+1][j]==0) ) out[i][j]=-5;
			else if ( (i>0)   && (in[i-1][j]==0) ) out[i][j]=-5;
			else if ( (j<y-1) && (in[i][j+1]==0) ) out[i][j]=-5;
			else if ( (j>0)   && (in[i][j-1]==0) ) out[i][j]=-5;
			else out[i][j]=-INFINITY;				

		}
	}
}


void dv_compute(short** p, int x, int y)
{
	// template
	
	// size
	int tx = 3, ty = 3;
	// data
	int upT[3][3] = { 
	{14,10,-1},
	{10, 0,-1},
	{14,-1,-1}}; 
/*
	{14,10,-1},
	{10, 0,-1},
	{14,-1,-1}}; 
*/

	int downT[3][3] = { 
	{-1,-1,14},
	{-1, 0,10},
	{-1,10,14}}; 
/*
	{-1,-1,14},
	{-1, 0,10},
	{-1,10,14}}; 
*/

	int min, max; // minimum/maximum value of sum of the matrix/template
	int tmp; // if the position is outside of array boundaries then return as if on the position was infinity

	int i,j;
	
	// 1st chamfering
	for (j=0; j<y; j++)
	for (i=0; i<x; i++) {

		// don't change anything if it is object or voxel next to the object
		if ( (p[i][j]!=5) && (p[i][j]!=-5) ) {

			// minimum value to infinity
			min = INFINITY;
			max = -INFINITY;

			// traverse pixels of template different than -1
			for (ty=-1; ty<2; ty++)
			for (tx=-1; tx<2; tx++) {
				if (upT[tx+1][ty+1]!=-1) {

					// check if it is positive or negative number
					if (p[i][j]>0) {

						// edge cases handling
						if ( (i+tx<0) || (i+tx>=x) || (j+ty<0) || (j+ty>=y) ) tmp = INFINITY; else tmp = p[i+tx][j+ty];
						// store smallest sum in the mask
						if (min>upT[tx+1][ty+1]+tmp) min=upT[tx+1][ty+1]+tmp;

					} else {

						if ( (i+tx<0) || (i+tx>=x) || (j+ty<0) || (j+ty>=y) ) tmp = -INFINITY; else tmp = p[i+tx][j+ty];
						if (max<tmp-upT[tx+1][ty+1]) max=tmp-upT[tx+1][ty+1];

					}

				}
			}
			// set new distance to the object
			if (p[i][j]>0) p[i][j]=min; else p[i][j]=max;
		}
	}

	// 2nd chamfering
	for (j=y-1; j>=0; j--)
	for (i=x-1; i>=0; i--) {
		// don't change anything if it is object or voxel next to the object
		if ( (p[i][j]!=5) && (p[i][j]!=-5) ) {
				
			// minimum value to infinity
			min = INFINITY;
			max = -INFINITY;

			// traverse pixels of template different than -1
			for (ty=-1; ty<2; ty++)
				for (tx=-1; tx<2; tx++) {
					if (downT[tx+1][ty+1]!=-1) {

						// check if it is positive or negative number
						if (p[i][j]>0) {

							// edge cases handling
							if ( (i+tx<0) || (i+tx>=x) || (j+ty<0) || (j+ty>=y) ) tmp = INFINITY; else tmp = p[i+tx][j+ty];
							// store smallest sum in the mask
							if (min>downT[tx+1][ty+1]+tmp) min=downT[tx+1][ty+1]+tmp;

						} else {

							if ( (i+tx<0) || (i+tx>=x) || (j+ty<0) || (j+ty>=y) ) tmp = -INFINITY; else tmp = p[i+tx][j+ty];
							if (max<tmp-downT[tx+1][ty+1]) max=tmp-downT[tx+1][ty+1];

						}
					}
				}
			// set new distance to the object
			if (p[i][j]>0) p[i][j]=min; else p[i][j]=max;
		}
	}

}



void dv_celldist(short** dv, int x, int y, unsigned char **p) // input:  distance volume (dv) 2d array of distances voxels and its size
{
	int min; // minimum/avg from distance 2x2 cell

	// traverse all cells of 2d array
	for (int j=0; j<y-1; j++)
	for (int i=0; i<x-1; i++) {

		// is there at least one object (negative value) in the cell ? 
		if ( (dv[i][j]<0) || (dv[i+1][j]<0) || (dv[i][j+1]<0) || (dv[i+1][j+1]<0) ) p[i][j]=0;
		else {

			// search minimum distance; avg gets better approximation (lower number of incorrect distances)
			//min=INFINITY;	// min distance
			min=0;			// avg distance
			
			// traverse cell 2x2
			for (int ty=0; ty<2; ty++)
			for (int tx=0; tx<2; tx++) {
				//if (min>dv[i+tx][j+ty]) min=dv[i+tx][j+ty];
				min+=dv[i+tx][j+ty];
			}

			// stored values are type of real. convert to integeter. result is increased by 1
			//min=(min/10)+1;
			min=min/40; 
			if (min>255) min=255; // final array stores 8bit distance at maximum

			p[i][j]=min;
		}			
	}
}

