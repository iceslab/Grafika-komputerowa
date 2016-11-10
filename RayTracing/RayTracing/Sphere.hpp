#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_

#define flow(array, dir) array[0] dir array[1] dir array[2]

#include <math.h>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>

typedef float Point3D[3];
typedef float Float3[3];

typedef float Color3[3];
using namespace std;

class Sphere
{
private:

public:

	Point3D position;
	float radius;
	Float3 specular;
	Float3 diffuse;
	Float3 ambient;
	float shine;


	Sphere(){};
	friend istream& operator>>(istream& in, Sphere& s);
	friend ostream& operator<<(ostream& in, Sphere& s);

};
#endif