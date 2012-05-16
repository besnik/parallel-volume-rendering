// matica.cpp
// by skip

#include "../include/matica.h"

// konstruktory

CMatica4::CMatica4()
{
	_[0][0] = 1.0;  _[0][1] = 0.0;  _[0][2] = 0.0;  _[0][3] = 0.0;
	_[1][0] = 0.0;  _[1][1] = 1.0;  _[1][2] = 0.0;  _[1][3] = 0.0;
	_[2][0] = 0.0;  _[2][1] = 0.0;  _[2][2] = 1.0;  _[2][3] = 0.0;
	_[3][0] = 0.0;  _[3][1] = 0.0;  _[3][2] = 0.0;  _[3][3] = 1.0;
}




CMatica4::CMatica4(float _00, float _01, float _02, float _03,
													float _10, float _11, float _12, float _13, 
													float _20, float _21, float _22, float _23,
				  								float _30, float _31, float _32, float _33 )
{
	_[0][0] = _00;  _[0][1] = _01;  _[0][2] = _02;  _[0][3] = _03;
	_[1][0] = _10;  _[1][1] = _11;  _[1][2] = _12;  _[1][3] = _13;
	_[2][0] = _20;  _[2][1] = _21;  _[2][2] = _22;  _[2][3] = _23;
	_[3][0] = _30;  _[3][1] = _31;  _[3][2] = _32;  _[3][3] = _33; 
}




CMatica4::CMatica4(const CMatica4& mat)
{
	_[0][0] = mat._[0][0];  _[0][1] = mat._[0][1];  _[0][2] = mat._[0][2];  _[0][3] = mat._[0][3];
	_[1][0] = mat._[1][0];  _[1][1] = mat._[1][1];  _[1][2] = mat._[1][2];  _[1][3] = mat._[1][3];
	_[2][0] = mat._[2][0];  _[2][1] = mat._[2][1];  _[2][2] = mat._[2][2];  _[2][3] = mat._[2][3];
	_[3][0] = mat._[3][0];  _[3][1] = mat._[3][1];  _[3][2] = mat._[3][2];  _[3][3] = mat._[3][3];
}




// pretazene unarne operatory


CMatica4 CMatica4::operator+() const
{
	return *this;
}




CMatica4 CMatica4::operator-() const
{
	return CMatica4(-_[0][0], -_[0][1], -_[0][2], -_[0][3],
								  -_[1][0], -_[1][1], -_[1][2], -_[1][3],
								  -_[2][0], -_[2][1], -_[2][2], -_[2][3],
								  -_[3][0], -_[3][1], -_[3][2], -_[3][3]);
}




CMatica4 CMatica4::operator*(float val) const
{
return CMatica4(_[0][0]*val, _[0][1]*val, _[0][2]*val, _[0][3]*val,
								_[1][0]*val, _[1][1]*val, _[1][2]*val, _[1][3]*val,
								_[2][0]*val, _[2][1]*val, _[2][2]*val, _[2][3]*val,
								_[3][0]*val, _[3][1]*val, _[3][2]*val, _[3][3]*val );
}





CMatica4 operator*(float val, const CMatica4& mat)
{
	return CMatica4(val*mat._[0][0], val*mat._[0][1], val*mat._[0][2], val*mat._[0][3],
									val*mat._[1][0], val*mat._[1][1], val*mat._[1][2], val*mat._[1][3],
									val*mat._[2][0], val*mat._[2][1], val*mat._[2][2], val*mat._[2][3],
									val*mat._[3][0], val*mat._[3][1], val*mat._[3][2], val*mat._[3][3] );
}




CMatica4 CMatica4::operator/(float val) const
{
	if(ABS(val) > EPSILON)
	{
		float inv_val = 1.0f/val;

		return CMatica4(_[0][0]*inv_val, _[0][1]*inv_val, _[0][2]*inv_val, _[0][3]*inv_val,
										_[1][0]*inv_val, _[1][1]*inv_val, _[1][2]*inv_val, _[1][3]*inv_val,
										_[2][0]*inv_val, _[2][1]*inv_val, _[2][2]*inv_val, _[2][3]*inv_val,
										_[3][0]*inv_val, _[3][1]*inv_val, _[3][2]*inv_val, _[3][3]*inv_val );
	} else return CMatica4();
}




// pretazene binarne operatory

CMatica4 CMatica4::operator+(const CMatica4& mat) const
{
	return CMatica4(_[0][0]+mat._[0][0], _[0][1]+mat._[0][1], _[0][2]+mat._[0][2], _[0][3]+mat._[0][3],
									_[1][0]+mat._[1][0], _[1][1]+mat._[1][1], _[1][2]+mat._[1][2], _[1][3]+mat._[1][3],
									_[2][0]+mat._[2][0], _[2][1]+mat._[2][1], _[2][2]+mat._[2][2], _[2][3]+mat._[2][3],
									_[3][0]+mat._[3][0], _[3][1]+mat._[3][1], _[3][2]+mat._[3][2], _[3][3]+mat._[3][3] );
}
  
  
  
  
CMatica4 CMatica4::operator-(const CMatica4& mat) const
{
	return CMatica4( _[0][0]-mat._[0][0], _[0][1]-mat._[0][1], _[0][2]-mat._[0][2], _[0][3]-mat._[0][3],
									 _[1][0]-mat._[1][0], _[1][1]-mat._[1][1], _[1][2]-mat._[1][2], _[1][3]-mat._[1][3],
									 _[2][0]-mat._[2][0], _[2][1]-mat._[2][1], _[2][2]-mat._[2][2], _[2][3]-mat._[2][3],
									 _[3][0]-mat._[3][0], _[3][1]-mat._[3][1], _[3][2]-mat._[3][2], _[3][3]-mat._[3][3] );
}
  
  
  
  
CMatica4 CMatica4::operator*(const CMatica4& mat) const
{
  return CMatica4( _[0][0]*mat._[0][0] + _[0][1]*mat._[1][0] + _[0][2]*mat._[2][0] + _[0][3]*mat._[3][0], 
									 _[0][0]*mat._[0][1] + _[0][1]*mat._[1][1] + _[0][2]*mat._[2][1] + _[0][3]*mat._[3][1],
									 _[0][0]*mat._[0][2] + _[0][1]*mat._[1][2] + _[0][2]*mat._[2][2] + _[0][3]*mat._[3][2],
									 _[0][0]*mat._[0][3] + _[0][1]*mat._[1][3] + _[0][2]*mat._[2][3] + _[0][3]*mat._[3][3],
									 _[1][0]*mat._[0][0] + _[1][1]*mat._[1][0] + _[1][2]*mat._[2][0] + _[1][3]*mat._[3][0],
									 _[1][0]*mat._[0][1] + _[1][1]*mat._[1][1] + _[1][2]*mat._[2][1] + _[1][3]*mat._[3][1],
									 _[1][0]*mat._[0][2] + _[1][1]*mat._[1][2] + _[1][2]*mat._[2][2] + _[1][3]*mat._[3][2],
									 _[1][0]*mat._[0][3] + _[1][1]*mat._[1][3] + _[1][2]*mat._[2][3] + _[1][3]*mat._[3][3],
 									 _[2][0]*mat._[0][0] + _[2][1]*mat._[1][0] + _[2][2]*mat._[2][0] + _[2][3]*mat._[3][0],
									 _[2][0]*mat._[0][1] + _[2][1]*mat._[1][1] + _[2][2]*mat._[2][1] + _[2][3]*mat._[3][1],
									 _[2][0]*mat._[0][2] + _[2][1]*mat._[1][2] + _[2][2]*mat._[2][2] + _[2][3]*mat._[3][2],
									 _[2][0]*mat._[0][3] + _[2][1]*mat._[1][3] + _[2][2]*mat._[2][3] + _[2][3]*mat._[3][3],
									 _[3][0]*mat._[0][0] + _[3][1]*mat._[1][0] + _[3][2]*mat._[2][0] + _[3][3]*mat._[3][0],
									 _[3][0]*mat._[0][1] + _[3][1]*mat._[1][1] + _[3][2]*mat._[2][1] + _[3][3]*mat._[3][1],
									 _[3][0]*mat._[0][2] + _[3][1]*mat._[1][2] + _[3][2]*mat._[2][2] + _[3][3]*mat._[3][2],
									 _[3][0]*mat._[0][3] + _[3][1]*mat._[1][3] + _[3][2]*mat._[2][3] + _[3][3]*mat._[3][3] );
}




CVektor3 CMatica4::operator*(const CVektor3& v) const
{
	CVektor3 w;
  w.x = _[0][0] * v.x + _[0][1] * v.y + _[0][2] * v.z;
  w.y = _[1][0] * v.x + _[1][1] * v.y + _[1][2] * v.z;
  w.z = _[2][0] * v.x + _[2][1] * v.y + _[2][2] * v.z;
  return w;
}




CVektor3 operator*(const CVektor3& v, const CMatica4& mat)
{
	CVektor3 w;
	w.x = mat._[0][0] * v.x + mat._[1][0] * v.y + mat._[2][0] * v.z;
	w.y = mat._[0][1] * v.x + mat._[1][1] * v.y + mat._[2][1] * v.z;
	w.z = mat._[0][2] * v.x + mat._[1][2] * v.y + mat._[2][2] * v.z;
	return w;
}




bool CMatica4::operator==(const CMatica4& mat) const
{
	return _[0][0] == mat._[0][0] && _[0][1] == mat._[0][1] && _[0][2] == mat._[0][2] && _[0][3] == mat._[0][3] &&
				 _[1][0] == mat._[1][0] && _[1][1] == mat._[1][1] && _[1][2] == mat._[1][2] && _[1][3] == mat._[1][3] &&
	   		 _[2][0] == mat._[2][0] && _[2][1] == mat._[2][1] && _[2][2] == mat._[2][2] && _[2][3] == mat._[2][3] &&
	   		 _[3][0] == mat._[3][0] && _[3][1] == mat._[3][1] && _[3][2] == mat._[3][2] && _[3][3] == mat._[3][3]; 
}
  
  
  
  
bool CMatica4::operator!=(const CMatica4& mat) const 
{
	return _[0][0] != mat._[0][0] || _[0][1] != mat._[0][1] || _[0][2] != mat._[0][2] || _[0][3] != mat._[0][3] ||
				 _[1][0] != mat._[1][0] || _[1][1] != mat._[1][1] || _[1][2] != mat._[1][2] || _[1][3] != mat._[1][3] ||
	   		 _[2][0] != mat._[2][0] || _[2][1] != mat._[2][1] || _[2][2] != mat._[2][2] || _[2][3] != mat._[2][3] ||
	   		 _[3][0] != mat._[3][0] || _[3][1] != mat._[3][1] || _[3][2] != mat._[3][2] || _[3][3] != mat._[3][3]; 
}



// ostatne funkcie, pre rychlost su mimo triedy


CMatica4 vynuluj()
{
	return CMatica4(0.0, 0.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 0.0,
									0.0, 0.0, 0.0, 0.0 );
}




CMatica4 identita()
{
	return CMatica4(1.0, 0.0, 0.0, 0.0,
									0.0, 1.0, 0.0, 0.0,
									0.0, 0.0, 1.0, 0.0,
									0.0, 0.0, 0.0, 1.0 );
}




CVektor3 vynasob(const CVektor3& v, const CMatica4& mat)
{
	float a = v.x*mat._[0][3]+v.y*mat._[1][3]+v.z*mat._[2][3];

	if(ABS( a ) >= EPSILON)	// bod
	{ 
		a = 1.0f/a;
		return CVektor3(( v.x*mat._[0][0]+v.y*mat._[1][0]+v.z*mat._[2][0] )*a,
										( v.x*mat._[0][1]+v.y*mat._[1][1]+v.z*mat._[2][1] )*a,
										( v.x*mat._[0][2]+v.y*mat._[1][2]+v.z*mat._[2][2] )*a );
	} else	// vektor
	{ 
		return CVektor3(v.x*mat._[0][0]+v.y*mat._[1][0]+v.z*mat._[2][0],
										v.x*mat._[0][1]+v.y*mat._[1][1]+v.z*mat._[2][1],
										v.x*mat._[0][2]+v.y*mat._[1][2]+v.z*mat._[2][2] );
	}
}


CMatica4 vynasobMatice(const CMatica4& mat1, const CMatica4& mat2)
{
  return CMatica4(mat1._[0][0]*mat2._[0][0] + mat1._[0][1]*mat2._[1][0] + mat1._[0][2]*mat2._[2][0] + mat1._[0][3]*mat2._[3][0],
								 mat1._[0][0]*mat2._[0][1] + mat1._[0][1]*mat2._[1][1] + mat1._[0][2]*mat2._[2][1] + mat1._[0][3]*mat2._[3][1],
        				 mat1._[0][0]*mat2._[0][2] + mat1._[0][1]*mat2._[1][2] + mat1._[0][2]*mat2._[2][2] + mat1._[0][3]*mat2._[3][2],
								 mat1._[0][0]*mat2._[0][3] + mat1._[0][1]*mat2._[1][3] + mat1._[0][2]*mat2._[2][3] + mat1._[0][3]*mat2._[3][3],
								 mat1._[1][0]*mat2._[0][0] + mat1._[1][1]*mat2._[1][0] + mat1._[1][2]*mat2._[2][0] + mat1._[1][3]*mat2._[3][0],
								 mat1._[1][0]*mat2._[0][1] + mat1._[1][1]*mat2._[1][1] + mat1._[1][2]*mat2._[2][1] + mat1._[1][3]*mat2._[3][1],
								 mat1._[1][0]*mat2._[0][2] + mat1._[1][1]*mat2._[1][2] + mat1._[1][2]*mat2._[2][2] + mat1._[1][3]*mat2._[3][2],
								 mat1._[1][0]*mat2._[0][3] + mat1._[1][1]*mat2._[1][3] + mat1._[1][2]*mat2._[2][3] + mat1._[1][3]*mat2._[3][3],
								 mat1._[2][0]*mat2._[0][0] + mat1._[2][1]*mat2._[1][0] + mat1._[2][2]*mat2._[2][0] + mat1._[2][3]*mat2._[3][0],
								 mat1._[2][0]*mat2._[0][1] + mat1._[2][1]*mat2._[1][1] + mat1._[2][2]*mat2._[2][1] + mat1._[2][3]*mat2._[3][1],
								 mat1._[2][0]*mat2._[0][2] + mat1._[2][1]*mat2._[1][2] + mat1._[2][2]*mat2._[2][2] + mat1._[2][3]*mat2._[3][2],
								 mat1._[2][0]*mat2._[0][3] + mat1._[2][1]*mat2._[1][3] + mat1._[2][2]*mat2._[2][3] + mat1._[2][3]*mat2._[3][3],
								 mat1._[3][0]*mat2._[0][0] + mat1._[3][1]*mat2._[1][0] + mat1._[3][2]*mat2._[2][0] + mat1._[3][3]*mat2._[3][0],
								 mat1._[3][0]*mat2._[0][1] + mat1._[3][1]*mat2._[1][1] + mat1._[3][2]*mat2._[2][1] + mat1._[3][3]*mat2._[3][1],
								 mat1._[3][0]*mat2._[0][2] + mat1._[3][1]*mat2._[1][2] + mat1._[3][2]*mat2._[2][2] + mat1._[3][3]*mat2._[3][2],
								 mat1._[3][0]*mat2._[0][3] + mat1._[3][1]*mat2._[1][3] + mat1._[3][2]*mat2._[2][3] + mat1._[3][3]*mat2._[3][3] );
}



CMatica4 transpozicia(const CMatica4& mat)
{
	return CMatica4( mat._[0][0], mat._[1][0], mat._[2][0], mat._[3][0],
                   mat._[0][1], mat._[1][1], mat._[2][1], mat._[3][1],
                   mat._[0][2], mat._[1][2], mat._[2][2], mat._[3][2],
                   mat._[0][3], mat._[1][3], mat._[2][3], mat._[3][3] );
}




float determinant(const CMatica4& mat)
{
  float tmp[12], a[4];

	tmp[0] = mat._[2][2]*mat._[3][3];
	tmp[1] = mat._[2][3]*mat._[3][2];
	tmp[2] = mat._[2][1]*mat._[3][3];
	tmp[3] = mat._[2][3]*mat._[3][1];
	tmp[4] = mat._[2][1]*mat._[3][2];
	tmp[5] = mat._[2][2]*mat._[3][1];
	tmp[6] = mat._[2][0]*mat._[3][3];
	tmp[7] = mat._[2][3]*mat._[3][0];
	tmp[8] = mat._[2][0]*mat._[3][2];
	tmp[9] = mat._[2][2]*mat._[3][0];
	tmp[10]= mat._[2][0]*mat._[3][1];
	tmp[11]= mat._[2][1]*mat._[3][0];

	a[0] = mat._[1][1]*tmp[0] + mat._[1][2]*tmp[3] + mat._[1][3]*tmp[4]-
				 mat._[1][3]*tmp[5] - mat._[1][1]*tmp[1] - mat._[1][2]*tmp[2];

	a[1] = mat._[1][3]*tmp[9] + mat._[1][0]*tmp[1] + mat._[1][2]*tmp[6]-
				 mat._[1][0]*tmp[0] - mat._[1][2]*tmp[7] - mat._[1][3]*tmp[8];

	a[2] = mat._[1][0]*tmp[2] + mat._[1][1]*tmp[7] + mat._[1][3]*tmp[10]-
				 mat._[1][3]*tmp[11] - mat._[1][0]*tmp[3] - mat._[1][1]*tmp[6];

	a[3] = mat._[1][2]*tmp[11] + mat._[1][0]*tmp[5] + mat._[1][1]*tmp[8]-
				 mat._[1][0]*tmp[4] - mat._[1][1]*tmp[9] - mat._[1][2]*tmp[10];

	return mat._[0][0]*a[0]+mat._[0][1]*a[1]+mat._[0][2]*a[2]+mat._[0][3]*a[3];
}



CMatica4 inverzna(const CMatica4& mat)
{
	CMatica4 inv_mat;
	float tmp[12], det;


	tmp[0] = mat._[2][2]*mat._[3][3];
	tmp[1] = mat._[3][2]*mat._[2][3];
	tmp[2] = mat._[1][2]*mat._[3][3];
	tmp[3] = mat._[3][2]*mat._[1][3];
	tmp[4] = mat._[1][2]*mat._[2][3];
	tmp[5] = mat._[2][2]*mat._[1][3];
	tmp[6] = mat._[0][2]*mat._[3][3];
	tmp[7] = mat._[3][2]*mat._[0][3];
	tmp[8] = mat._[0][2]*mat._[2][3];
	tmp[9] = mat._[2][2]*mat._[0][3];
	tmp[10]= mat._[0][2]*mat._[1][3];
	tmp[11]= mat._[1][2]*mat._[0][3];

	inv_mat._[0][0] = tmp[0]*mat._[1][1] + tmp[3]*mat._[2][1] + tmp[4]*mat._[3][1] -
										tmp[1]*mat._[1][1] - tmp[2]*mat._[2][1] - tmp[5]*mat._[3][1];
	inv_mat._[0][1] = tmp[1]*mat._[0][1] + tmp[6]*mat._[2][1] + tmp[9]*mat._[3][1] -
										tmp[0]*mat._[0][1] - tmp[7]*mat._[2][1] - tmp[8]*mat._[3][1];
	inv_mat._[0][2] = tmp[2]*mat._[0][1] + tmp[7]*mat._[1][1] + tmp[10]*mat._[3][1] -
										tmp[3]*mat._[0][1] - tmp[6]*mat._[1][1] - tmp[11]*mat._[3][1];
  inv_mat._[0][3] = tmp[5]*mat._[0][1] + tmp[8]*mat._[1][1] + tmp[11]*mat._[2][1] -
										tmp[4]*mat._[0][1] - tmp[9]*mat._[1][1] - tmp[10]*mat._[2][1];
	inv_mat._[1][0] = tmp[1]*mat._[1][0] + tmp[2]*mat._[2][0] + tmp[5]*mat._[3][0] -
										tmp[0]*mat._[1][0] - tmp[3]*mat._[2][0] - tmp[4]*mat._[3][0];
	inv_mat._[1][1] = tmp[0]*mat._[0][0] + tmp[7]*mat._[2][0] + tmp[8]*mat._[3][0] -
										tmp[1]*mat._[0][0] - tmp[6]*mat._[2][0] - tmp[9]*mat._[3][0];
	inv_mat._[1][2] = tmp[3]*mat._[0][0] + tmp[6]*mat._[1][0] + tmp[11]*mat._[3][0] -
										tmp[2]*mat._[0][0] - tmp[7]*mat._[1][0] - tmp[10]*mat._[3][0];
  inv_mat._[1][3] = tmp[4]*mat._[0][0] + tmp[9]*mat._[1][0] + tmp[10]*mat._[2][0] -
										tmp[5]*mat._[0][0] - tmp[8]*mat._[1][0] - tmp[11]*mat._[2][0];

	tmp[0] = mat._[2][0]*mat._[3][1];
	tmp[1] = mat._[3][0]*mat._[2][1];
	tmp[2] = mat._[1][0]*mat._[3][1];
	tmp[3] = mat._[3][0]*mat._[1][1];
	tmp[4] = mat._[1][0]*mat._[2][1];
	tmp[5] = mat._[2][0]*mat._[1][1];
	tmp[6] = mat._[0][0]*mat._[3][1];
	tmp[7] = mat._[3][0]*mat._[0][1];
	tmp[8] = mat._[0][0]*mat._[2][1];
	tmp[9] = mat._[2][0]*mat._[0][1];
	tmp[10]= mat._[0][0]*mat._[1][1];
	tmp[11]= mat._[1][0]*mat._[0][1];

	inv_mat._[2][0] = tmp[0]*mat._[1][3] + tmp[3]*mat._[2][3] + tmp[4]*mat._[3][3]-
										tmp[1]*mat._[1][3] - tmp[2]*mat._[2][3] - tmp[5]*mat._[3][3];
	inv_mat._[2][1] = tmp[1]*mat._[0][3] + tmp[6]*mat._[2][3] + tmp[9]*mat._[3][3]-
										tmp[0]*mat._[0][3] - tmp[7]*mat._[2][3] - tmp[8]*mat._[3][3];
	inv_mat._[2][2] = tmp[2]*mat._[0][3] + tmp[7]*mat._[1][3] + tmp[10]*mat._[3][3]-
										tmp[3]*mat._[0][3] - tmp[6]*mat._[1][3] - tmp[11]*mat._[3][3];
  inv_mat._[2][3] = tmp[5]*mat._[0][3] + tmp[8]*mat._[1][3] + tmp[11]*mat._[2][3]-
										tmp[4]*mat._[0][3] - tmp[9]*mat._[1][3] - tmp[10]*mat._[2][3];
	inv_mat._[3][0] = tmp[2]*mat._[2][2] + tmp[5]*mat._[3][2] + tmp[1]*mat._[1][2]-
										tmp[4]*mat._[3][2] - tmp[0]*mat._[1][2] - tmp[3]*mat._[2][2];
	inv_mat._[3][1] = tmp[8]*mat._[3][2] + tmp[0]*mat._[0][2] + tmp[7]*mat._[2][2]-
										tmp[6]*mat._[2][2] - tmp[9]*mat._[3][2] - tmp[1]*mat._[0][2];
	inv_mat._[3][2] = tmp[6]*mat._[1][2] + tmp[11]*mat._[3][2] + tmp[3]*mat._[0][2]-
										tmp[10]*mat._[3][2] - tmp[2]*mat._[0][2] - tmp[7]*mat._[1][2];
  inv_mat._[3][3] = tmp[10]*mat._[2][2] + tmp[4]*mat._[0][2] + tmp[9]*mat._[1][2]-
										tmp[8]*mat._[1][2] - tmp[11]*mat._[2][2] - tmp[5]*mat._[0][2];

	det = mat._[0][0]*inv_mat._[0][0]+mat._[1][0]*inv_mat._[0][1]+mat._[2][0]*inv_mat._[0][2]+mat._[3][0]*inv_mat._[0][3];
	det = 1.0f/det;
  inv_mat = inv_mat * det;

	return inv_mat;
}




CMatica4 posun(const CVektor3& t)
{
	CMatica4 mat;

	mat._[0][0] = 1.0;  mat._[0][1] = 0.0;  mat._[0][2] = 0.0;  mat._[0][3] = 0.0;
	mat._[1][0] = 0.0;  mat._[1][1] = 1.0;  mat._[1][2] = 0.0;  mat._[1][3] = 0.0;
	mat._[2][0] = 0.0;  mat._[2][1] = 0.0;  mat._[2][2] = 1.0;  mat._[2][3] = 0.0;
	mat._[3][0] = t.x;  mat._[3][1] = t.y;  mat._[3][2] = t.z;  mat._[3][3] = 1.0;

	return mat;
}




CMatica4 skaluj(const CVektor3& s)
{
	CMatica4 mat;

	mat._[0][0] = s.x;  mat._[0][1] = 0.0;  mat._[0][2] = 0.0;  mat._[0][3] = 0.0;
	mat._[1][0] = 0.0;  mat._[1][1] = s.y;  mat._[1][2] = 0.0;  mat._[1][3] = 0.0;
	mat._[2][0] = 0.0;  mat._[2][1] = 0.0;  mat._[2][2] = s.z;  mat._[2][3] = 0.0;
	mat._[3][0] = 0.0;  mat._[3][1] = 0.0;  mat._[3][2] = 0.0;  mat._[3][3] = 1.0;

	return mat; 
}




CMatica4 rotateX(float a)
{
	float b = sinf( a ), c = cosf( a );
	CMatica4 mat;

	mat._[0][0] = 1.0;  mat._[0][1] = 0.0;  mat._[0][2] = 0.0;  mat._[0][3] = 0.0;
	mat._[1][0] = 0.0;  mat._[1][1] = c;    mat._[1][2] = b;    mat._[1][3] = 0.0;
	mat._[2][0] = 0.0;  mat._[2][1] = -b;   mat._[2][2] = c;    mat._[2][3] = 0.0;
	mat._[3][0] = 0.0;  mat._[3][1] = 0.0;  mat._[3][2] = 0.0;  mat._[3][3] = 1.0;

	return mat;
}


CMatica4 rotateY(float a)
{
	float b = sinf( a ), c = cosf( a );
	CMatica4 mat;

	mat._[0][0] = c;    mat._[0][1] = 0.0;  mat._[0][2] = -b;   mat._[0][3] = 0.0;
	mat._[1][0] = 0.0;  mat._[1][1] = 1.0;  mat._[1][2] = 0.0;  mat._[1][3] = 0.0;
	mat._[2][0] = b;    mat._[2][1] = 0.0;  mat._[2][2] = c;    mat._[2][3] = 0.0;
	mat._[3][0] = 0.0;  mat._[3][1] = 0.0;  mat._[3][2] = 0.0;  mat._[3][3] = 1.0;

	return mat;
}



CMatica4 rotateZ(float a)
{
	float b = sinf( a ), c = cosf( a );
	CMatica4 mat;

	mat._[0][0] = c;    mat._[0][1] = b;    mat._[0][2] = 0.0;  mat._[0][3] = 0.0;
	mat._[1][0] = -b;   mat._[1][1] = c;    mat._[1][2] = 0.0;  mat._[1][3] = 0.0;
	mat._[2][0] = 0.0;  mat._[2][1] = 0.0;  mat._[2][2] = 1.0;  mat._[2][3] = 0.0;
	mat._[3][0] = 0.0;  mat._[3][1] = 0.0;  mat._[3][2] = 0.0;  mat._[3][3] = 1.0;

	return mat;
}



CMatica4 euler(float head, float pitch, float roll) 
{
	return rotateY( head )*rotateX( pitch )*rotateZ( roll );
}



CMatica4 orthographic(float l, float r, float b, float t, float n, float f)
{
	CMatica4 mat;

	mat._[0][0] = 2.0f/( r-l );      mat._[0][1] = 0.0;              mat._[0][2] = 0.0;          mat._[0][3] = 0.0;
	mat._[1][0] = 0.0;              mat._[1][1] = 2.0f/( t-b );      mat._[1][2] = 0.0;          mat._[1][3] = 0.0;
	mat._[2][0] = 0.0;              mat._[2][1] = 0.0;              mat._[2][2] = 1.0f/( f-n );  mat._[2][3] = 0.0;
	mat._[3][0] = ( r+l )/( l-r );  mat._[3][1] = ( t+b )/( b-t );  mat._[3][2] = n/( n-f );    mat._[3][3] = 1.0;

	return mat;
}



CMatica4 perspective(float l, float r, float b, float t, float n, float f)
{
	CMatica4 mat;

	mat._[0][0] = ( 2.0f*n )/( r-l );  mat._[0][1] = 0.0;                mat._[0][2] = 0.0;              mat._[0][3] = 0.0;
	mat._[1][0] = 0.0;                mat._[1][1] = ( 2.0f*n )/( t-b );  mat._[1][2] = 0.0;              mat._[1][3] = 0.0;
	mat._[2][0] = ( r+l )/( l-r );    mat._[2][1] = ( t+b )/( b-t );    mat._[2][2] = f/( f-n );        mat._[2][3] = 1.0;
	mat._[3][0] = 0.0;                mat._[3][1] = 0.0;                mat._[3][2] = ( f*n )/( n-f );  mat._[3][3] = 0.0;

	return mat;
}



CMatica4 view(const CVektor3& C, const CVektor3& n, const CVektor3& v, bool lh)
{
	CVektor3 u, n_, v_;
	CMatica4 mat;

	n_ = normalizuj(n);
	v_ = normalizuj(v-n_*skalarsucin(v,n_));
	if (lh == true) u = normalizuj(vektsucin(n_,v_)); else u = -normalizuj(vektsucin(n_,v_));

	mat._[0][0] = u.x; mat._[0][1] = v_.x; mat._[0][2] = n_.x; mat._[0][3] = 0.0;
	mat._[1][0] = u.y; mat._[1][1] = v_.y; mat._[1][2] = n_.y; mat._[1][3] = 0.0;
	mat._[2][0] = u.z; mat._[2][1] = v_.z; mat._[2][2] = n_.z; mat._[2][3] = 0.0;
	
	mat._[3][0] = -skalarsucin(CVektor3( C.x, C.y, C.z ), u );
  mat._[3][1] = -skalarsucin(CVektor3( C.x, C.y, C.z ), v_ );
  mat._[3][2] = -skalarsucin(CVektor3( C.x, C.y, C.z ), n_ );
  mat._[3][3] = 1.0;

	return mat;
}
