// vektor3.h
// Trieda CVektor3, zakladne operacie s vektorom
// By skippo
// Version 1.05 [optimalizovana]

#ifndef __VEKTOR3_H
#define __VEKTOR3_H	

#include "definicie.h"
#include <math.h>

class CVektor3
{
public:
	float x, y, z;
	
	CVektor3();
	CVektor3(const CVektor3& v);
	explicit CVektor3(float x, float y, float z);						// konstruktor, explicit: nepouzivanie jednoparametrickeho konstruktoru k implicitnemu pretypovaniu. napr nemozeme spravit v1 = v2 + 5 (vektor + cislo (ostatne zlozky y,z su defaulntne nastavene na 0))
	
	CVektor3 operator+() const;															// unarne pretazene +
	CVektor3 operator-() const;															// unarne pretazene -
	
	CVektor3 operator+(float f) const;
	CVektor3& operator+=(float f);
	CVektor3 operator-(float f) const;
	CVektor3 operator*(float f) const;
  friend CVektor3 operator+(float f, const CVektor3& v);
	friend CVektor3 operator-(float f, const CVektor3& v);
	friend CVektor3 operator*(float f, const CVektor3& v);
	CVektor3 operator/(float f) const;
	CVektor3 operator*(const CVektor3& v) const;
	CVektor3 operator+(const CVektor3& v) const;						// binarne pretazene +, ako parameter referencia: rychlejsie je predat 8B bez xB(kolko zabera objekt)
	CVektor3 operator-(const CVektor3& v) const;						// binarne pretazene -	
	CVektor3& operator=(const CVektor3& v);

	bool operator==(const CVektor3& v) const;
	bool operator!=(const CVektor3& v) const;
};


float dlzka(const CVektor3& v);
CVektor3 normalizuj(const CVektor3& v);	// normalizacia vektora, dlzka == 1

float skalarsucin(const CVektor3& u, const CVektor3& v);	// skalarny sucin
CVektor3 vektsucin(const CVektor3& u, const CVektor3& v);	// vektorovy sucin

void rotujX(CVektor3& v, float uhol);	// rotacia okolo osi x
void rotujY(CVektor3& v, float uhol);	// rotacia okolo osi y
void rotujZ(CVektor3& v, float uhol);	// rotacia okolo osi z
CVektor3 rotateAroundArbitrary(const CVektor3& v, CVektor3& os, float uhol);	// rotacia vektora okolo osi o uhol (v stupnoch)
float uholVektorov(const CVektor3& v, const CVektor3& w);	// vrati uhol medzi dvoma vektormi, v *radianoch*
float vzdialenost(const CVektor3& v, const CVektor3& w);	// vzialenost dvoch vektorov
CVektor3 halfway(const CVektor3& u, const CVektor3& v);	// half vektor 


#endif



// TODO
// bude vyhodena funckia pripocitaj nakolko uz mame pretazene +
// porozmyslat ako vhodne pretazit * aby sme mohli vyhodit napr vekt sucin (/), alebo skal sucin (*)




/*
 
Historia Verzii

==================
1.06  [04.01.2006]
==================

	- dalsie funkcie a pretazene operatory


==================
1.05  [26.12.2005]
==================

	- optimalizacie:
	- premenne su namiesto double -> float
	- vsetky funkcie okrem pretazenych mimo class
	- vsetky funkcie mimo class su inline
	- pridane dalsie pretazene funckie ako * a /, =
	- vymazane niektore fcie z povodnej verzie 1.04

==================
1.04  [4.09.2005]
==================

	- pretazena cast operatorov

==================
1.03  [16.08.2005]
==================

	- pripocitaj iny vektor ku vektoru
	- opravene rotacie
	- vypis vektora na console output

==================
1.00  [01.08.2005]
==================

	- zakladne funkcie ako sk. a ve. sucin, normalizacia, dlzka vektora

*/
