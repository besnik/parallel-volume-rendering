// funkcie.cpp
// by skip

#include "funkcie.h"


float logn(float x, float base)
{
	return logf(x)/logf(base);
}



float znamienko(float x)
{
	if( x < -EPSILON ) return -1.0f; else
	if( x > EPSILON ) return 1.0f; else return 0.0f;
}



float mocnina(float x)
{
	return x*x;
}




float odmocnina(float x)
{
	return x >= 0.0f ? powf( x, 0.5f ) : 0.0f;
}




float orez(float x, float min, float max)
{
	if( x < min ) return min; else if( x > max ) return max; else return x;  
}




float zaokruhli(float x)
{
	return x-(long)x < 0.5f ? (float)(long)x : (float)(long)x+1;
}




float random(float min, float max)
{
	return min+( max-min )*( ( rand() % 32001 )*0.00003125f );
}



float diV(float x, float y, float c)
{
	return ABS(y) < EPSILON ? c : x/y;
}




float exP(float x)
{
	return powf((float)EULER_NUMBER, x);
}




UINT32 dvaNaX32(UINT8 x)
{
	switch(x)
	{
		case 0: return 1; 
		case 1: return 2; 
		case 2: return 4; 
		case 3: return 8; 
		case 4: return 16; 
		case 5: return 32;
		case 6: return 64;
		case 7: return 128;
		case 8: return 256;
		case 9: return 512;
		case 10: return 1024;
		case 11: return 2048;
		case 12: return 4096;
		case 13: return 8192;
		case 14: return 16384;
		case 15: return 32768;
		case 16: return 65536;
		case 17: return 131072;
		case 18: return 262144;
		case 19: return 524288;
		case 20: return 1048576;
		case 21: return 2097152;
		case 22: return 4194304;
		case 23: return 8388608;
		case 24: return 16777216;
		case 25: return 33554432;
		case 26: return 67108864;
		case 27: return 134217728;
		case 28: return 268435456;
		case 29: return 536870912;
		case 30: return 1073741824;
		//case 31: return 2147483648; break; // toto je unsigned iba v ISO C90
		// uf
		default: return 0; 
	}
}




float linInt(UINT16 a, UINT16 b, float fa, float fb, UINT16 x)
{
	if (b != a) 
	{
		return fa + (fb - fa) * float(x - a) / float(b - a);
	}
	else // delenie nulou
	{
		return 0.0f;
	} 
}



float triInt(const float& v000, const float& v100,
						 const float& v101, const float& v001,
						 const float& v010, const float& v110,
						 const float& v111, const float& v011,
						 float x, float y, float z)
{
	if (x<EPSILON) x=0.0f;
	if (y<EPSILON) y=0.0f;
	if (z<EPSILON) z=0.0f;
	return v000*(1.0f-x)*(1.0f-y)*(1.0f-z)+
				 v100*x*(1.0f-y)*(1.0f-z)+
				 v010*(1.0f-x)*y*(1.0f-z)+
				 v001*(1.0f-x)*(1.0f-y)*z+
				 v011*(1.0f-x)*y*z+
				 v101*x*(1.0f-y)*z+
				 v110*x*y*(1.0f-z)+
				 v111*x*y*z;
}



float schlick(float x, float n)
{
	return x/(n-n*x+x);
}




CVektor3 cent_dif(float fx1, float fx2, float fy1, float fy2, float fz1, float fz2)
{
	return CVektor3((fx1-fx2)/2.0f, (fy1-fy2)/2.0f, (fz1-fz2)/2.0f);
}




void vypis(const CVektor3& v)
{
	cout << "(" << v.x << "," << v.y << "," << v.z << ")";
}




// shading

CVektor3 getAnalGrad(const float& v000, const float& v100,
							       const float& v010, const float& v110,
							       const float& v001, const float& v101,
							       const float& v011, const float& v111,
										 const CVektor3& v)
{
	CVektor3 g;
	g.x=(1.0f-v.y)*(v100-v000 + v.z*(v011-v001+v000-v100))+(v.y)*(v110-v010+v.z*(v111-v101+v010-v110));
	g.y=(1.0f-v.x)*(v010-v000+v.z*(v101-v001+v000-v010))+(v.x)*(v110-v100+v.z*(v111-v011+v100-v110));
	g.z=(1.0f-v.x)*(v001-v000+v.y*(v101-v010+v000-v001))+(v.x)*(v011-v100+v.y*(v111-v110+v100-v011));
	return g;
}



UINT32 faktorial(UINT32 n)
{
   if (n==0) return 1;            
   else return n*faktorial(n-1);
}



UINT32 komCis(UINT32 n,UINT32 i)
{
	if ((i>n)) return 0;
	if ((i==0) || (i==n)) return 1;
	//if (n<13)  return Faktorial(n)/(Faktorial(i)*Faktorial(n-i));
	//else return 0;    //int32 zvladne maximalne 12! udrzat !!!!
	//(10/6)
	UINT32 citatel=1,menovatel=1;

	if (i<n-i) i=n-i;
	int j;
	for (j=n;j>i;j--) citatel*=j;
	for (j=1;j<=(n-i);j++) menovatel*=j;
	return (UINT32)(citatel/menovatel);
}
