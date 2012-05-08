// ray.cpp
// by skip

#include "../include/ray.h"




// konstruktory

CRay::CRay()
{
	pos = dir = CVektor3();
}




CRay::CRay(const CVektor3& pos, const CVektor3& dir)
{
	this->pos = pos;
	this->dir = dir;
}




CRay::CRay(const CRay& r)
{
	pos = r.pos;
	dir = r.dir;
}




// pretazene unarne operatory

CRay CRay::operator+() const
{
	return *this;
}




CRay CRay::operator-() const
{
	return CRay(-pos, -dir);
}




// pretazene binarne operatory

bool CRay::operator==(const CRay& r) const
{
	return pos == r.pos && dir == r.dir;
}




bool CRay::operator!=(const CRay& r) const
{
	return pos != r.pos || dir != r.dir;
}






