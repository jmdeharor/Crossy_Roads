/* ------------------------------------------------------

 Vec3 - Lighthouse3D

  -----------------------------------------------------*/


#ifndef _VEC3_
#define _VEC3_

#include <glm\glm.hpp>

class Vec3  
{

public:

	float x,y,z;


	Vec3::Vec3(float x, float y, float z);
	Vec3::Vec3(const Vec3 &v);
	Vec3::Vec3(const glm::vec3& v);
	Vec3::Vec3();
	Vec3::~Vec3();

	Vec3 Vec3::operator +(const Vec3 &v) const;
	Vec3 Vec3::operator -(const Vec3 &v) const;
	Vec3 Vec3::operator *(const Vec3 &v) const;
	Vec3 Vec3::operator *(float t) const;
	Vec3 Vec3::operator /(float t) const;
	Vec3 Vec3::operator -(void) const;

	float Vec3::length() const;
	void Vec3::normalize();
	float Vec3::innerProduct(const Vec3  &v) const;
	void copy(const Vec3 &v);
	void set(float x,float y, float z);
	Vec3 scalarMult(float a) const;

	void Vec3::print() const;

};


#endif