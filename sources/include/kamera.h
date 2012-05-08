// Camera
// supports orthogonal rendering. functionality for perspective redering is missing (set zoom, etc)
// does not support camera matrix

#ifndef __KAMERA_H
#define __KAMERA_H


#include "matica.h"

class CKamera
{
public:
	// camera plane, popisana vektormi u,v; je popisana "z=0" v camera space
	CVektor3 from;	// pozicia kamery
	CVektor3 to;	// definuje smer kamery "dir" - pouziva sa IBA na definovanie "Dir" v konstruktore
	CVektor3 u;	// horizontalny vektor camera plane, x-ova strana obrazovky
	CVektor3 v;	// vertikalny vektor camera plane, y-ova strana obrazovky		
	CVektor3 dir;	// smer kamery
	CVektor3 up; // drzime ho iba pre overenie spravneho nastavenia parametrov kamery
	float zoom;	// velkost kamery
	float n, f;	// predna a zadna orezavacia rovina, vzhladom na vektor "dir"

	//CMatica4 camMatica;	// matica kamery, nic ine ako vektory u,v a dir 
	//CMatica4 invCamMatica;	// inverzna matica kamery

	// konstruktory
	CKamera();
	CKamera(const CVektor3& from, const CVektor3& to, const CVektor3& up, float zoom, float n, float f);

	void reset();	// resetne hodnoty kamery na 0
	void kopiruj(CKamera *c);	// skopiruje *svoje* hodnoty do kamery "c"
	void move(const CVektor3& offset);	// posunie "from" o "offset"
	void rotuj(CVektor3& os, float uhol);	// rotuje kameru okolo niektorej osi o uhol
	// nastavenie smeru pohladu
	void setLookDir(const CVektor3& newDir);	// kamera sa bude pozerat urcenym smerom
	void lookAt(const CVektor3& ciel);	// kamera sa bude pozerat na zadany bod
	float zistiVzd(const CVektor3& v);	// zisti vzialenost kamery od bodu
	// rotacie; uhol v stupnoch
	void yawPlanet(float uhol);	// rotuje okolo osi Y ako v FPS hre - teda okolo osi 0,1,0
	void yawSpace(float uhol);	// rotuje okolo vlastnej osi ako vo vesmirnom simulatore
	void pitch(float uhol);	// rotuje okolo osi X 1,0,0
	void roll(float uhol);	// rotuje okolo osi Z 0,0,1
	// ratios - zisti a nastavi screen ratio (e.g. 4:3, 640:480, ...)
	float getRatioUV();	
	float getRatioVU();
	void setRatioUV(float ratio);
	void setRatioVU(float ratio);
	// scale - zmenit skalovanie kamery (dir,u,v) bez zmeny co vidime
	void scale(float faktor);
	float getScale();
	void setScale(float dirDlzka);
	



};


#endif




// version 1.0 - 27.12.2005
// zakladna struktura a operacie
