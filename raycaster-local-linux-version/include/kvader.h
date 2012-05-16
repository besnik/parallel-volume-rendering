// kvader.h
// trieda CKvader - reprezentuje kvader v priestore, funkcie na prienik luca s kvadrom
// by skip
// Version 1.00


#ifndef __KVADER_H
#define __KVADER_H


#include "ray.h"

class CKvader
{
public:
	CVektor3 min, max;	// lavy dolny predny roh a opacne

	CKvader();
	CKvader(float minx, float miny, float minz, float maxx, float maxy, float maxz);
	CKvader(const CVektor3& min, const CVektor3& max);
	CKvader(const CKvader& k);

	bool operator==(const CKvader& k) const;
	bool operator!=(const CKvader& k) const;
};


// prienik kvadra a luca/bodu, t/tmin/tmax == cim vynasobit smer luca aby sme dostali prienik
bool kvaderBod(const CKvader& k, const CVektor3& b);	// bod v kocke
bool kvaderRay(const CKvader& k, const CRay& r, float *tmin_, float *tmax_);
bool kvaderRay(const CKvader& k, const CRay& r);
bool kvaderRay(const CKvader& k, const CRay& r, float *t);




#endif


/*

Version History

	1.0
	28.12.2005
	zakladne operacie a prieniky s bodom/lucom

*/
