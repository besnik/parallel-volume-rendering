// Basic operations with ray

#ifndef __RAY_H
#define __RAY_H


#include "../include/vektor3.h"


class CRay
{
public:
	CVektor3 pos;	// startovacia pozicia luca
	CVektor3 dir;	// smer luca
	//konstruktory
	CRay();
	CRay(const CVektor3& pos, const CVektor3& dir);
	CRay(const CRay& r);

	CRay operator+(void) const;
	CRay operator-(void) const;

	bool operator==(const CRay& r) const;
	bool operator!=(const CRay& r) const;
};


#endif


/*

	version history:
	1.0 - 27.12.2005. zakladne operacie s lucom

*/


