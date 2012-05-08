// basic operations with color
// conversion from RGB to HSV and vice versa
// color interpolation, mixing of colors, cutting, calculation of intensity, etc

#ifndef __FARBA_H
#define __FARBA_H

#include "definicie.h"

typedef unsigned int   DWORD;


class CFarba{
public:
	float r,g,b,a;
	// konstruktory
	CFarba();
	CFarba(DWORD argb);
	CFarba(float r, float g, float b, float a);
	CFarba(const CFarba& c);
	// pretypovanie cfarba na dword
	operator DWORD() const;

	CFarba operator+() const;
	CFarba operator-() const;

	CFarba operator*(float val) const;
	friend CFarba operator*(float val, const CFarba& c);
	CFarba operator/(float val) const;

	CFarba operator+(const CFarba& c) const;
	CFarba operator-(const CFarba& c) const;
	bool operator==(const CFarba& c) const;
	bool operator!=(const CFarba& c) const;
};

// dalsie funckie na pracu s farbami

float getI(const CFarba& c);	// vrati intenzitu farby - skalar
CFarba orez(const CFarba& c, float a, float b);	// oreze farbu na interval <a,b>
CFarba mixuj(float x, const CFarba& cA, const CFarba& cB);	// linearna interpolacia 
CFarba mixujHermit(float x, const CFarba& cA, const CFarba& cB );	// hermitov polynom
CFarba mixujBilin(float x, float y, const CFarba& cA, const CFarba& cB, const CFarba& cC, const CFarba& cD);
CFarba rgb_hsv(const CFarba& rgb);
CFarba hsv_rgb(const CFarba& hsv);




#endif
