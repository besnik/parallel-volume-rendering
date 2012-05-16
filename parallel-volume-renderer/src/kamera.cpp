// kamera.cpp
// by skip

#include "../include/kamera.h"
#include "../include/funkcie.h"

// konstruktory

CKamera::CKamera()
{
	up = from = to = u = v = dir = CVektor3();
	n = f = 0.0f;
	zoom = 1.0f;
}




CKamera::CKamera(const CVektor3& from, const CVektor3& to, const CVektor3& up, float zoom, float n, float f)
{
	this->from = from;
	this->to   = to;
	this->zoom = zoom;
	this->n    = n;
	this->f    = f;
	this->up   = up;

	this->dir  = normalizuj(to - from);
	this->u    = normalizuj(vektsucin(up,dir));
	this->v    = normalizuj(vektsucin(dir,u));
}




void CKamera::reset()
{
	up = from = to = u = v = dir = CVektor3();
	zoom = n = f = 0.0f;
}




void CKamera::kopiruj(CKamera *c)
{
	if (c)
	{
		c->reset();
		c->from = from;
		c->to   = to;
		c->dir  = dir;
		c->u    = u;
		c->v    = v;
		c->n    = n;
		c->f    = f;
		c->zoom = zoom;
		c->up   = up;
	}
}




void CKamera::move(const CVektor3& offset)
{
	from = from + offset;
}




void CKamera::rotuj(CVektor3& os, float uhol)
{
	u = rotateAroundArbitrary(u, os, uhol);
	v = rotateAroundArbitrary(v, os, uhol);
	dir = rotateAroundArbitrary(dir, os, uhol);
}




// nastavenia pohladu kamery, meni *u,v,dir*
void CKamera::setLookDir(const CVektor3& newDir)
{
	CVektor3 os = vektsucin(dir, newDir);	// vytvori os okolo ktorej sa bude rotovat, os je kolma na oba vektory
	if(dlzka(os) == 0) return;
	float uhol = uholVektorov(dir, newDir);	// o aky uhol budeme otacat "dir" okolo osi
	if(uhol != 0.0f) rotuj(os, uhol);   
}




void CKamera::lookAt(const CVektor3& ciel)
{
	setLookDir(ciel - from); 
}




float CKamera::zistiVzd(const CVektor3& v)
{
	return vzdialenost(from, v);
}




// rotacie kamery

void CKamera::yawPlanet(float uhol)
{
	CVektor3 v = CVektor3(0,1,0);
	rotuj(v, uhol);
}




void CKamera::yawSpace(float uhol)
{
	rotuj(v,uhol);
}




void CKamera::pitch(float uhol)
{
	rotuj(u,uhol);
}




void CKamera::roll(float uhol)
{
	rotuj(dir,uhol);
}




// ratios

float CKamera::getRatioUV()
{
	return dlzka(u)/dlzka(v);
}




float CKamera::getRatioVU()
{
	return dlzka(v)/dlzka(u);
}




void CKamera::setRatioUV(float ratio)
{
	v = normalizuj(v);
	v = v*dlzka(u)/ratio;	// normalizovany vektor v * jeho dlzka
}




void CKamera::setRatioVU(float ratio)
{
	u = normalizuj(u);
	u = u*dlzka(v)/ratio;
}




float CKamera::getScale()
{
	return dlzka(dir);
}




void CKamera::scale(float faktor)
{
	dir = dir * faktor;
	u   = u   * faktor;
	v   = v   * faktor;
}




void CKamera::setScale(float dirDlzka)
{
	scale(dlzka(dir)/dirDlzka);
}





