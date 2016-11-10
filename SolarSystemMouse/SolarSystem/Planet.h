#pragma once
#include "stdafx.h"
#include <vector>
using namespace std;

class Planet
{
public:
	Planet(float firstAxis, float secondAxis, float radius, float startingAngleVelocity, angle3 spinSpeed, float startingAngle = 0.0f);
	Planet(const Planet &p);
	Planet& operator =(const Planet &p);

	Planet* operator[](size_t el);

	void getPosition(point2& pos);
	void getWorldPosition(point3& pos);
	float getRadius();

	void setPlane(angle3 angles);
	void setPlane(GLfloat angle1, GLfloat angle2, GLfloat angle3);

	void setColor(color3 colors);
	void setColor(GLfloat color1, GLfloat color2, GLfloat color3);

	void setModelMatrix(float* modelMatrix);

	void setName(string name);
	string getName();

	void nextStep();

	void addChild(Planet &p);

	void draw(const bool &path = false);
	void calcAccuratePosition();
	friend ostream & operator<< (ostream &os, const Planet &p);

private:
	float minorAxis, majorAxis, radius;
	float area, p, e, angle, ellipseStep;
	angle3 angles, spinSpeed, planeAngles;
	point2 position;
	color3 color;
	vector<Planet*> children;
	string name;

	float modelMatrix[16];
	float transformedMatrix[16];

	void clone(const Planet &p);

	void prepareMatrix(const bool &path);
	void translate();
	void rotate();
	void rotatePlane();
	void drawPath();
	void drawChildren(const bool &path);

	static float _r(float p, float e, float degreesAngle);
};