#pragma once

class c_vec {
public:
	float x, y, z;

	inline c_vec() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	inline c_vec(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	inline c_vec &operator=(const c_vec &vOther)
	{
		x = vOther.x;
		y = vOther.y;
		z = vOther.z;
		return *this;
	}
	
	inline float &operator[](int i)
	{
		return ((float*)this)[i];
	}

	inline float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	inline bool operator==(const c_vec &src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}

	inline bool operator!=(const c_vec &src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	inline c_vec &operator+=(const c_vec &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline c_vec &operator-=(const c_vec &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline c_vec &operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	inline c_vec &operator*=(const c_vec &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	inline c_vec	&operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}
	//===============================================
	inline c_vec	&operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}
	//===============================================
	inline c_vec &operator/=(float fl)
	{
		float oofl = 1.0f / fl;
		x *= oofl;
		y *= oofl;
		z *= oofl;
		return *this;
	}

	inline c_vec &operator/=(const c_vec &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	inline c_vec operator+(const c_vec &v) const
	{
		c_vec res;
		res.x = x + v.x;
		res.y = y + v.y;
		res.z = z + v.z;
		return res;
	}

	
	inline c_vec operator-(const c_vec &v) const
	{
		c_vec res;
		res.x = x - v.x;
		res.y = y - v.y;
		res.z = z - v.z;
		return res;
	}
	
	inline c_vec operator*(float fl) const
	{
		c_vec res;
		res.x = x * fl;
		res.y = y * fl;
		res.z = z * fl;
		return res;
	}
	
	inline c_vec operator*(const c_vec &v) const
	{
		c_vec res;
		res.x = x * v.x;
		res.y = y * v.y;
		res.z = z * v.z;
		return res;
	}
	
	inline c_vec operator/(float fl) const
	{
		c_vec res;
		res.x = x / fl;
		res.y = y / fl;
		res.z = z / fl;
		return res;
	}
	
	inline c_vec operator/(const c_vec &v) const
	{
		c_vec res;
		res.x = x / v.x;
		res.y = y / v.y;
		res.z = z / v.z;
		return res;
	}
};