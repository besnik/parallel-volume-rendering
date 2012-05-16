// shader.h
// by skip
// class CShader, contains color of the material, functions for shading object
// note: I would rename the type to CMaterial to better express meaning behind it
// version 1.0

#ifndef __SHADER_H
#define __SHADER_H


#include "farba.h"
#include "vektor3.h"
#include "funkcie.h"

class CShader
{
public:
	// material
	CFarba ambient; // okolne svetlo
	CFarba diffuse; // rozptilene svetlo
	CFarba specular; // odrazene svetlo
	
	float spec_exp; // odrazovy exponent
	float Fa, Fd, Fs; // koeficienty svetla
	
	// konstruktory
	CShader();
	CShader(const CFarba& a, const CFarba& d, const CFarba& s, float spec_exp,
		      float Fa, float Fd, float Fs);
};


// tienovacie funkcie


CFarba tienujPhong(const CFarba& farba, const CShader& shader,
									 const CVektor3& L, const CVektor3& V, const CVektor3& N); // farba=vstupna farba, L=light vector, N=normala, V=view vector




#endif


/*

Version History

	7.1.2006
	- basic class and shading function was created

*/
