// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  float rad = degrees/180.0*pi;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	glm::mat3 I(1.0);
	glm::mat3 rot1(x*x,x*y,x*z,x*y,y*y,y*z,x*z,y*z,z*z);
	rot1 = glm::transpose(rot1); 
	glm::mat3 rot2(0, -z, y, z, 0, -x, -y, x, 0);
	rot2 = glm::transpose(rot2);
	glm::mat3 rotation = cos(rad)*I + (1-cos(rad))*rot1 + sin(rad)*rot2;
	// You will change this return call
	return rotation;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.  
  mat3 rotation = rotate(degrees,glm::normalize(up));
	eye = rotation*eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  // YOUR CODE FOR HW2 HERE 
  // Likely the same as in HW 1. 
  vec3 u = -glm::normalize(glm::cross(up, eye));
	mat3 rotation = rotate(degrees, u);
	eye = rotation*eye;
	up = rotation*up; 
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);
	mat4 lookAtMatrix(
	u.x, u.y, u.z, -glm::dot(u,eye),
	v.x, v.y, v.z, -glm::dot(v,eye),
	w.x, w.y, w.z, -glm::dot(w,eye),
	0,   0,   0,   1.0f
	); 
	lookAtMatrix = glm::transpose(lookAtMatrix);
	// You will change this return call
	return lookAtMatrix;

  // vec3 c = glm::normalize(eye-center);
	// vec3 b = glm::normalize(up);
	// vec3 a = glm::cross(b, c);
  // mat4 transposeMatrix(
  //   1.0f, 0, 0, -eye.x,
  //   0, 1.0f, 0, -eye.y,
  //   0, 0, 1.0f, -eye.z,
  //   0, 0, 0, 1.0f
  // );
  // transposeMatrix = glm::transpose(transposeMatrix);
	// mat4 lookAtMatrix(
	// a.x, a.y, a.z, 0,
	// b.x, b.y, b.z, 0,
	// c.x, c.y, c.z, 0,
	// 0,   0,   0,   1.0f
	// ); 
	// lookAtMatrix = glm::transpose(lookAtMatrix);
	// You will change this return call
	//return lookAtMatrix*transposeMatrix;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  float rad = fovy/360*pi;
  float d = 1/tan(rad);
  mat4 perspectiveMatrix(
    d/aspect, 0, 0, 0,
    0, d, 0, 0,
    0, 0, -(zFar+zNear)/(zFar-zNear), -2*zFar*zNear/(zFar-zNear),
    0, 0, -1.0f, 0
  );
  perspectiveMatrix = glm::transpose(perspectiveMatrix);
  return perspectiveMatrix;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 scaleMatrix(
    sx, 0, 0, 0,
    0, sy, 0, 0,
    0, 0, sz, 0,
    0, 0, 0, 1.0f
  );
  scaleMatrix = glm::transpose(scaleMatrix);
  return scaleMatrix;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 tMatrix(
    1.0f, 0, 0, tx,
    0, 1.0f, 0, ty,
    0, 0, 1.0f, tz,
    0, 0, 0, 1.0f
  );
  tMatrix = glm::transpose(tMatrix);
  return tMatrix;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
