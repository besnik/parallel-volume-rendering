// vektor3.cpp
// Trieda CVektor3, zakladne operacie s vektorom
// By skippo, vid. historiu verzii v .h

#include "vektor3.h"



// konstruktory

CVektor3::CVektor3(void)
{
	x = y = z = 0.0f;
}


CVektor3::CVektor3(float x, float y, float z)		
{
	this->x = x; this->y = y; this->z = z;
}


CVektor3::CVektor3(const CVektor3& v)
{
	x=v.x; y=v.y; z=v.z;
}




// pretazene unarne funkcie


CVektor3 CVektor3::operator+() const
{
	return *this;
}


CVektor3 CVektor3::operator-() const
{
	return CVektor3(-x, -y, -z);
}


CVektor3 CVektor3::operator*(float f) const
{
	return CVektor3(x*f, y*f, z*f);
}




CVektor3 CVektor3::operator+(float f) const
{
	return CVektor3(x+f, y+f, z+f);
}




CVektor3& CVektor3::operator +=(float f) 
{
	x+=f; y+=f; z+=f;
	return *this;
}

CVektor3 CVektor3::operator-(float f) const
{
	return CVektor3(x*f, y*f, z*f);
}




CVektor3 operator+(float f, const CVektor3& v )
{
	return CVektor3(f+v.x, f+v.y, f+v.z);
}




CVektor3 operator-(float f, const CVektor3& v )
{
	return CVektor3(f-v.x, f-v.y, f-v.z);
}





CVektor3 operator*(float f, const CVektor3& v )
{
	return CVektor3(f*v.x, f*v.y, f*v.z);
}


CVektor3 CVektor3::operator/(float f) const 
{
	if( fabs( f ) > EPSILON )
	{
		float inv_f = 1.0f/f;

		return CVektor3( x*inv_f, y*inv_f, z*inv_f );
	}
	else return CVektor3();
}




CVektor3 CVektor3::operator*(const CVektor3 &v) const		
{
	return CVektor3(x*v.x, y*v.y, z*v.z);
}




CVektor3 CVektor3::operator-(const CVektor3 &v) const			// binarne pretazene -
{
	return CVektor3(x-v.x, y-v.y, z-v.z);
}


CVektor3 CVektor3::operator+(const CVektor3 &v) const			// binarne pretazene +
{
	return CVektor3(x+v.x, y+v.y, z+v.z);
}


CVektor3& CVektor3::operator=(const CVektor3& v)
{
	x = v.x; y = v.y; z = v.z;
	return *this;
}


bool CVektor3::operator==(const CVektor3 &v) const
{
	return x == v.x && y == v.y && z == v.z;
}


bool CVektor3::operator!=(const CVektor3 &v) const
{
	return x != v.x || y != v.y || z != v.z;
}




// ostatne funkcie, pre rychlost su mimo triedy



float dlzka(const CVektor3& v)							// dlzka vektora
{
	return (float)sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}


CVektor3 normalizuj(const CVektor3& v)
{
	float d = (float)sqrt(v.x*v.x + v.y*v.y + v.z*v.z);				// dlzka v
	
	if( fabs( d ) < EPSILON )
	{
      return CVektor3();
    } 
	else
	{
      d = 1.0f/d;

      return CVektor3( v.x*d, v.y*d, v.z*d );
    } 	
}


float skalarsucin(const CVektor3& u, const CVektor3&v)
{
  return u.x*v.x + u.y*v.y + u.z*v.z;
}


CVektor3 vektsucin(const CVektor3& u, const CVektor3&v)
{
  return CVektor3( u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x );
}


void rotujX(CVektor3& v, float uhol)				// rotacia vektora okolo osi X
{
	float oldy = v.y;
	uhol = uhol * (float)DEG_RAD_PRESNE;						// vstup do sin(), cos() je v radianoch!
	
	v.y = v.y * cosf(uhol) - v.z * sinf(uhol);
	v.z = oldy * sinf(uhol) + v.z * cosf(uhol);
}


void rotujY(CVektor3& v, float uhol)				// rotacia okolo osi Y
{
	float oldx = v.x;
	uhol = uhol * (float)DEG_RAD_PRESNE;						// vstup do sin(), cos() je v radianoch!
		
	v.x = v.z * sinf(uhol) + v.x * cosf(uhol);
	v.z = v.z * cosf(uhol) - oldx * sinf(uhol);
}


void rotujZ(CVektor3& v, float uhol)				// rotacia okolo osi Z
{
	float oldx = v.x;
	uhol = uhol * (float)DEG_RAD_PRESNE;
	
	v.x = v.x * cosf(uhol) - v.y * sinf(uhol);
	v.y = oldx * sinf(uhol) + v.y * cosf(uhol);
}




CVektor3 rotateAroundArbitrary(const CVektor3& v, CVektor3& os, float uhol)
{
	if((v.x == 0) && (v.y == 0) && (v.z == 0)) return CVektor3();

	CVektor3 w;
	float c, s, t;

	os = normalizuj(os);

	// vypocet parametrov rotacnej matice
	uhol = uhol * (float)DEG_RAD_PRESNE;
	c = cosf(uhol);
	s = sinf(uhol);
	t = 1.0f - c;

	// vynasob v s rotacnou maticou
	w.x = (t * os.x * os.x +          c) * v.x
			+ (t * os.x * os.y + s * os.z) * v.y
			+ (t * os.x * os.z - s * os.y) * v.z;
       
	w.y = (t * os.x * os.y - s * os.z) * v.x 
			+ (t * os.y * os.y +          c) * v.y 
			+ (t * os.y * os.z + s * os.x) * v.z;
       
	w.z = (t * os.x * os.z + s * os.y) * v.x 
			+ (t * os.y * os.z - s * os.x) * v.y 
			+ (t * os.z * os.z +          c) * v.z;

	w = normalizuj(w);
	w = w * dlzka(v);
    
	return w;
}




float uholVektorov(const CVektor3& v, const CVektor3& w)
{
	  //dot product(v,w) = length(v)*length(w)*cos(angle) ==> angle = acos(dot/(length(v)*length(w))) = acos(dot(norm(v)*norm(w)));
    float cosineOfAngle = skalarsucin(normalizuj(v), normalizuj(w));
    //for acos, the value has to be between -1.0 and 1.0, but due to numerical imprecisions it sometimes comes outside this range
    if (cosineOfAngle > 1.0) cosineOfAngle = 1.0;
    if (cosineOfAngle < -1.0) cosineOfAngle = -1.0;
    // vystup v radianoch
		return -acosf(cosineOfAngle);
}




float vzdialenost(const CVektor3& v, const CVektor3& w)
{
	return sqrtf((v.x - w.x) * (v.x - w.x) + (v.y - w.y)	* (v.y - w.y) + (v.z - w.z) * (v.z - w.z));
}




CVektor3 halfway(const CVektor3& u, const CVektor3& v)
{
	return normalizuj(u+v);
}



