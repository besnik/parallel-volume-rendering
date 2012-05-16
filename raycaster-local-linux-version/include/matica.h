// matica.h
// Trieda CMatica, operacie s maticou
// By skippo
// Version 1.00 

#ifndef __MATICA_H
#define __MATICA_H

#include "vektor3.h"

class CMatica4
{
public:
	float _[4][4];

	CMatica4();
	CMatica4( float _00, float _01, float _02, float _03,
		        float _10, float _11, float _12, float _13, 
			      float _20, float _21, float _22, float _23,
				  	float _30, float _31, float _32, float _33 );
	
	CMatica4(const CMatica4& mat);

	CMatica4 operator+() const;
	CMatica4 operator-() const;

	CMatica4 operator*(float val) const;
	friend CMatica4 operator*(float val, const CMatica4& mat); 
	CMatica4 operator/(float val) const;

	CMatica4 operator+(const CMatica4& mat) const;
	CMatica4 operator-(const CMatica4& mat) const;
	CMatica4 operator*(const CMatica4& mat) const;

	CVektor3 operator*(const CVektor3& v) const;
	friend CVektor3 operator*(const CVektor3& v, const CMatica4& mat);

	bool operator==(const CMatica4& mat) const;
	bool operator!=(const CMatica4& mat) const;
};




CMatica4 vynuluj();
CMatica4 identita();

CVektor3 vynasob(const CVektor3& v, const CMatica4& mat);
CMatica4 vynasobMatice(const CMatica4& mat1, const CMatica4& mat2);
CMatica4 transpozicia(const CMatica4& mat);
float determinant(const CMatica4& mat);
CMatica4 inverzna(const CMatica4& mat);
CMatica4 posun(const CVektor3& t);
CMatica4 skaluj(const CVektor3& s);
CMatica4 rotateX(float a);
CMatica4 rotateY(float a);
CMatica4 rotateZ(float a);
CMatica4 euler(float head, float pitch, float roll); 
CMatica4 orthographic(float l, float r, float b, float t, float n, float f);
CMatica4 perspective(float l, float r, float b, float t, float n, float f);
CMatica4 view(const CVektor3& C, const CVektor3& n, const CVektor3& v, bool lh);



#endif



/*
 
Historia Verzii

==================
1.00  [26.12.2005]
==================

	- zakladne operacie s maticou 4x4 (kvoli homogennym suradniciam)
	- nasobenie matice s nehomogennym vektorom !! 3D*4D!! pozor dat!
*/
