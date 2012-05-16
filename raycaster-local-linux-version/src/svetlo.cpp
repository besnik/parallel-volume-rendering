// svetlo.cpp
// by skip


#include "../include/svetlo.h"


// konstruktory


CSvetlo::CSvetlo()
{
	typ = AMBIENT_LIGHT;
	pos = dir = CVektor3();
	farba = CFarba();
}




CSvetlo::CSvetlo(const CFarba& farba)
{
	typ = AMBIENT_LIGHT;
	pos = dir = CVektor3();
	this->farba = farba;
}



CSvetlo::CSvetlo(const CVektor3& pos, const CVektor3& dir, const CFarba& farba)
{
	typ = DIRECTIONAL_LIGHT;
	this->pos = pos;
	this->dir = dir;
	this->farba = farba;
}
