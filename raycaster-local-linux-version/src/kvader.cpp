// kvader.cpp
// by skip

#include "../include/kvader.h"


// konstruktory

CKvader::CKvader()
{
	min = max = CVektor3();
}




CKvader::CKvader(float minx,float miny, float minz, float maxx, float maxy, float maxz)
{
	min.x = minx; min.y = miny; min.z = minz;
	max.x = maxx; max.y = maxy; max.z = maxz;
}




CKvader::CKvader(const CVektor3& min, const CVektor3& max)
{
	this->min = min; this->max = max;
}




CKvader::CKvader(const CKvader& k)
{
	min = k.min; max = k.max;
}



// pretazene binarne operatory

bool CKvader::operator==(const CKvader& k) const
{
	return min == k.min && max == k.max;
}




bool CKvader::operator!=(const CKvader& k) const
{
	return min != k.min || max != k.max;
}



// prieniky s lucom a bodom

bool kvaderBod(const CKvader& k, const CVektor3& b)
{
	if (b.x < k.min.x || b.x > k.max.x ) return false; else
	if (b.y < k.min.y || b.y > k.max.y ) return false; else
	if (b.z < k.min.z || b.z > k.max.z ) return false; else return true;
}




bool kvaderRay(const CKvader& k, const CRay& r, float *tmin_, float *tmax_)
{
 float tmin, tmax, t1, t2, a;


	tmin = -INFINITY;  tmax = INFINITY;

	if (ABS( r.dir.x ) > EPSILON)
	{
		a = 1.0f/r.dir.x;
		t1 = ( k.max.x-r.pos.x )*a;    
		t2 = ( k.min.x-r.pos.x )*a;    
		if (t1 > t2)
		{
		  a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if (r.pos.x > k.max.x || r.pos.x < k.min.x) return false;

	if (ABS(r.dir.y) > EPSILON)
	{
		a = 1.0f/r.dir.y;
		t1 = (k.max.y-r.pos.y)*a;    
		t2 = (k.min.y-r.pos.y)*a;    
		if(t1 > t2)
		{
			a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if(r.pos.y > k.max.y || r.pos.y < k.min.y) return false;

	if(ABS(r.dir.z) > EPSILON)
	{
		a = 1.0f/r.dir.z;
		t1 = (k.max.z-r.pos.z )*a;    
		t2 = (k.min.z-r.pos.z )*a;    
		if(t1 > t2)
		{
	    a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if (r.pos.z > k.max.z || r.pos.z < k.min.z ) return false;

	
	*tmin_ = tmin;  *tmax_ = tmax;

	
	return true; 
}



bool kvaderRay(const CKvader& k, const CRay& r)
{
	float tmin, tmax, t1, t2, a;


	tmin = -INFINITY;  tmax = INFINITY;

	if (ABS(r.dir.x) > EPSILON)
	{
		a = 1.0f/r.dir.x;
		t1 = (k.max.x-r.pos.x )*a;    
		t2 = (k.min.x-r.pos.x )*a;    
		if(t1 > t2)
		{
		  a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if (r.pos.x > k.max.x || r.pos.x < k.min.x) return false;

	if (ABS(r.dir.y ) > EPSILON)
	{
		a = 1.0f/r.dir.y;
		t1 = (k.max.y-r.pos.y)*a;    
		t2 = (k.min.y-r.pos.y)*a;    
		if(t1 > t2)
		{
		  a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if(r.pos.y > k.max.y || r.pos.y < k.min.y) return false;

	if (ABS(r.dir.z) > EPSILON)
	{
		a = 1.0f/r.dir.z;
		t1 = (k.max.z-r.pos.z)*a;    
		t2 = (k.min.z-r.pos.z)*a;    
		if (t1 > t2)
		{
		  a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if (r.pos.z > k.max.z || r.pos.z < k.min.z ) return false;

	
	return true; 
}




bool kvaderRay(const CKvader& k, const CRay& r, float *t)
{
	float tmin, tmax, t1, t2, a;


	tmin = -INFINITY;  tmax = INFINITY;

	if (ABS(r.dir.x) > EPSILON)
	{
		a = 1.0f/r.dir.x;
		t1 = (k.max.x-r.pos.x)*a;    
		t2 = (k.min.x-r.pos.x)*a;    
		if (t1 > t2)
		{
			a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if (r.pos.x > k.max.x || r.pos.x < k.min.x) return false;

	if (ABS(r.dir.y) > EPSILON)
	{
		a = 1.0f/r.dir.y;
		t1 = (k.max.y-r.pos.y )*a;    
		t2 = (k.min.y-r.pos.y )*a;    
		if (t1 > t2)
		{
			a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if (r.pos.y > k.max.y || r.pos.y < k.min.y) return false;

	if (ABS(r.dir.z) > EPSILON)
	{
		a = 1.0f/r.dir.z;
		t1 = (k.max.z-r.pos.z)*a;    
		t2 = (k.min.z-r.pos.z)*a;    
		if (t1 > t2)
		{
			a = t1;
			t1 = t2;
			t2 = a;
		}
		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;
		if (tmin > tmax) return false;
		if (tmax < 0.0f) return false;
	}	else
	if (r.pos.z > k.max.z || r.pos.z < k.min.z) return false;

  if (tmin >= 0.0f) *t = tmin; else *t = tmax;

	return true; 
}
