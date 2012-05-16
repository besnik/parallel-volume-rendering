// shader.cpp
// by skip


#include "../include/shader.h"


CShader::CShader()
{
	ambient = diffuse = specular = CFarba();
	spec_exp = 0.0f;
	Fa = Fd = Fs = 0.0f;
}




CShader::CShader(const CFarba& a, const CFarba& d, const CFarba& s, float spec_exp, 
								 float Fa, float Fd, float Fs)
{
	// farby
	this->ambient=a;
	this->diffuse=d;
	this->specular=s;
	// index odrazu
	this->spec_exp=spec_exp;
	// koeficienty, kolko tej-onej farby sa prida do vyslednej farby
	this->Fa=Fa;
	this->Fd=Fd;
	this->Fs=Fs;
}



// tienovacie funkcie

CFarba tienujPhong(const CFarba& c, const CShader& shader,
									 const CVektor3& L, const CVektor3& V, const CVektor3& N) // predpoklada biele svetlo
{
	// 3 zlozky vyslednej farby
	float Ia, Id=0.0f, Is=0.0f;
	// okolite svetlo
	Ia = shader.Fa;
	if (N!=CVektor3(0.0f,0.0f,0.0f))
	{
		// rozptilene svetlo
		Id = MAX(skalarsucin(L,N),0.0f)*shader.Fd;
		// odrazene svetlo
		Is = MAX(schlick(skalarsucin(halfway(L,V),N),shader.spec_exp),0.0f)*shader.Fs;
	}
	return c*(Ia + Id + Is);
}
