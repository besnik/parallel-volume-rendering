// light
// some basic types of light

#ifndef __SVETLO_H
#define __SVETLO_H


#include "farba.h"
#include "vektor3.h"


#undef AMBIENT_LIGHT
#undef DIRECTIONAL_LIGHT
#undef POINT_LIGHT 
#undef SPOT_LIGHT
#undef WARN_LIGHT

#define AMBIENT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT 2
#define SPOT_LIGHT 4
#define WARN_LIGHT 4



class CSvetlo
{
public:
	UINT8 typ; // jeden z hore definovanych typov
	CVektor3 pos; // pozicia
	CVektor3 dir; // smer
	CFarba farba;
	// konstruktory
	CSvetlo();
	CSvetlo(const CFarba& farba);
	CSvetlo(const CVektor3& pos, const CVektor3& dir, const CFarba& farba);
};



#endif



/*

	Version History

	1.0
		4.1.2006
		zakladna trieda a konstruktory

*/
