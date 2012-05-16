// farba.cpp
// by skip


#include "farba.h"
#include "funkcie.h"


CFarba::CFarba()
{
	a = r = g = b = 0.0;
}




CFarba::CFarba(DWORD argb)
{
	const float val = (float)0.00392156862745;	// 1.0/255.0

	r = val*(float)(unsigned char)( argb >> 16 );
	g = val*(float)(unsigned char)( argb >> 8 );
	b = val*(float)(unsigned char)( argb >> 0 );
	a = val*(float)(unsigned char)( argb >> 24 );
}




CFarba::CFarba(float r, float g, float b, float a)
{
	this->r = r; this->g = g; this->b = b; this->a = a;
}




CFarba::CFarba(const CFarba& c)
{
	r = c.r; g = c.g; b = c.b; a = c.a;
}




CFarba::operator DWORD() const
{
	DWORD dwR = r >= 1.0 ? 0xff : r <= 0.0 ? 0x00 : (DWORD)( r*255.0+0.5 );
	DWORD dwG = g >= 1.0 ? 0xff : g <= 0.0 ? 0x00 : (DWORD)( g*255.0+0.5 );
	DWORD dwB = b >= 1.0 ? 0xff : b <= 0.0 ? 0x00 : (DWORD)( b*255.0+0.5 );
	DWORD dwA = a >= 1.0 ? 0xff : a <= 0.0 ? 0x00 : (DWORD)( a*255.0+0.5 );

	return ( dwA << 24 ) | ( dwR << 16 ) | ( dwG << 8 ) | dwB;
} 




CFarba CFarba::operator+() const
{
	return *this;
}




CFarba CFarba::operator-() const
{
	return CFarba(-r,-g,-b,-a);
}




CFarba CFarba::operator*(float val) const
{
	return CFarba(r*val, g*val, b*val, a*val);
}




CFarba operator*(float val, const CFarba& c)
{
	return CFarba(val*c.r, val*c.g, val*c.b, val*c.a);
}




CFarba CFarba::operator/(float val) const
{
	if(ABS(val) > EPSILON)
	{
		float inv_val = 1.0f/val;
    return CFarba(r*inv_val, g*inv_val, b*inv_val, a*inv_val);
  } 
	else return CFarba();
}


CFarba CFarba::operator+(const CFarba& c) const
{
  return CFarba(r+c.r, g+c.g, b+c.b, a+c.a);
}




CFarba CFarba::operator-(const CFarba& c) const
{
  return CFarba(r-c.r, g-c.g, b-c.b, a-c.a);
}



bool CFarba::operator==(const CFarba& c) const
{
	return r == c.r && g == c.g && b == c.b && a == c.a; 
}




bool CFarba::operator!=(const CFarba& c) const
{
	return r != c.r || g != c.g || b != c.b || a != c.a;
}



float getI(const CFarba& c)
{
	return 0.299f*c.r+0.587f*c.g+0.114f*c.b;
}




CFarba orez(const CFarba& c, float a, float b)
{
	return CFarba(orez(c.r,a,b), orez(c.g,a,b), orez(c.b,a,b), orez(c.a,a,b));
}




CFarba mixuj(float x, const CFarba& cA, const CFarba& cB)
{
	return (1.0f-x)*cA+x*cB;
}



CFarba mixujHermit(float x, const CFarba& cA, const CFarba& cB)
{
	return cA+( cB-cA )*(3.0f*x*x-2.0f*x*x*x);
}




CFarba mixujBilin(float x, float y, const CFarba& cA, const CFarba& cB, const CFarba& cC, const CFarba& cD)
{
  return mixuj(y, mixuj(x, cA, cB), mixuj(x, cD, cC));
}


CFarba rgb_hsv(const CFarba& rgb)
{
  float min, max, delta;
  CFarba hsv;

  min = MIN(rgb.r, MIN( rgb.g, rgb.b ));
  max = MAX(rgb.r, MAX( rgb.g, rgb.b ));
  hsv.b = max;
  if( max == 0.0f ) hsv.g = 0.0f; else hsv.g = ( max-min )/max;

  if( hsv.g == 0.0f )
	{
	  hsv.r = -1.0f; // hsv.r = UNDEF
  } else
	{
    delta = max-min;
    if( rgb.r == max ) hsv.r = ( rgb.g-rgb.b )/delta;
		else if( rgb.g == max ) hsv.r = 2.0f+( rgb.b-rgb.r )/delta;
		else hsv.r = 4.0f+( rgb.r-rgb.g )/delta;
    hsv.r*=60.0f;
    if (hsv.r < 0.0f) hsv.r+=360.0f;
  }
  hsv.a = rgb.a;
  
  return hsv;
}




CFarba hsv_rgb(const CFarba& hsv)
{
  long i;
  float f, p, q, t;
  CFarba rgb;

  if( hsv.g == 0.0f )
	{ // hsv.r = UNDEF
      rgb = CFarba( hsv.b, hsv.b, hsv.b, hsv.a );
  } else
	{
    i = (long)floor( hsv.r*0.01666666f );
    f = hsv.r*0.01666666f-i;
    p = hsv.b*( 1.0f-hsv.g );
    q = hsv.b*( 1.0f-( hsv.g*f ) );
    t = hsv.b*( 1.0f-( hsv.g*( 1.0f-f ) ) );
    switch( i ) {
      case 0: rgb = CFarba( hsv.b, t, p, hsv.a ); break;
      case 1: rgb = CFarba( q, hsv.b, p, hsv.a ); break;
      case 2: rgb = CFarba( p, hsv.b, t, hsv.a ); break;
      case 3: rgb = CFarba( p, q, hsv.b, hsv.a ); break;
      case 4: rgb = CFarba( t, p, hsv.b, hsv.a ); break;
      case 5: rgb = CFarba( hsv.b, p, q, hsv.a ); break;
    }
  }

  return rgb;
}
