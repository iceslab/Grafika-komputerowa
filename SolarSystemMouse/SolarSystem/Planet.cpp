#include "Planet.h"

// PUBLIC
Planet::Planet(float firstAxis, float secondAxis, float radius, float startingAngleVelocity, angle3 spinSpeed, float startingAngle)
{
	if (firstAxis > secondAxis)
	{
		majorAxis = firstAxis;
		minorAxis = secondAxis;
	}
	else
	{
		majorAxis = secondAxis;
		minorAxis = firstAxis;
	}
	this->radius = radius;
	ellipseStep = startingAngleVelocity;
	p = pow(minorAxis, 2.0) / majorAxis;
	e = sqrt(1.0 - (pow(minorAxis, 2.0) / pow(majorAxis, 2.0)));
	angle = startingAngle;
	this->spinSpeed[0] = spinSpeed[0];
	this->spinSpeed[1] = spinSpeed[1];
	this->spinSpeed[2] = spinSpeed[2];

	angles[0] = spinSpeed[0];
	angles[1] = spinSpeed[1];
	angles[2] = 0.0;

	planeAngles[0] = 0.0;
	planeAngles[1] = 0.0;
	planeAngles[2] = 0.0;

	position[0] = 0.0;
	position[1] = 0.0;

	float r = _r(p, e, ellipseStep);
	area = (pow(r, 2.0f) * (ellipseStep * M_PI / 180.0f)) / 2.0f;
}

Planet::Planet(const Planet &p)
{
	clone(p);
}

Planet& Planet::operator =(const Planet &p)
{
	clone(p);
	return *this;
}

Planet* Planet::operator[](size_t el)
{
	return children[el];
}

void Planet::getPosition(point2& pos)
{
	pos[0] = position[0];
	pos[1] = position[1];

	/*GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	Matrix4 A(matrix);
	Vector4 V(position[0], position[1], 0.0, 1.0);
	Vector4 x = V * A;*/

	/*cout << A << endl;
	cout << V << endl;
	cout << x << endl;*/
}

void Planet::getWorldPosition(point3& pos)
{
	pos[0] = transformedMatrix[12];
	pos[1] = transformedMatrix[13];
	pos[2] = transformedMatrix[14];
}

float Planet::getRadius()
{
	return radius;
}

void Planet::setPlane(angle3 angles)
{
	planeAngles[0] = angles[0];
	planeAngles[1] = angles[1];
	planeAngles[2] = angles[2];
}

void Planet::setPlane(GLfloat angle1, GLfloat angle2, GLfloat angle3)
{
	planeAngles[0] = angle1;
	planeAngles[1] = angle2;
	planeAngles[2] = angle3;
}

void Planet::setColor(color3 colors)
{
	color[0] = colors[0];
	color[1] = colors[1];
	color[2] = colors[2];
}

void Planet::setColor(GLfloat color1, GLfloat color2, GLfloat color3)
{
	color[0] = color1;
	color[1] = color2;
	color[2] = color3;
}

void Planet::setModelMatrix(float* modelMatrix)
{
	for (int i = 0; i < 16; i++)
		this->modelMatrix[i] = modelMatrix[i];
}

void Planet::setName(string name)
{
	this->name = name;
}

string Planet::getName()
{
	return name;
}

void Planet::nextStep()
{
	if (majorAxis != 0.0 && minorAxis != 0.0)
	{
		float r = _r(p, e, angle);
		angle += ellipseStep;
		while (angle >= 360.0)
			angle -= 360.0;

		ellipseStep = (area * 2.0f) / pow(r, 2.0f);

		position[0] = r * cos(angle * M_PI / 180.0);
		position[1] = r * sin(angle * M_PI / 180.0);
	}

	angles[2] += spinSpeed[2];
	while (angles[2] >= 360.0)
		angles[2] -= 360.0;

	for (auto &p : children)
	{
		p->nextStep();
	}
}

void Planet::addChild(Planet &p)
{
	children.push_back(&p);
}

void Planet::draw(const bool &path)
{
	glPushMatrix();
	prepareMatrix(path);
	glColor3fv(color);
	glutWireSphere(radius, radius * 2, radius * 2);
	glPopMatrix();
}

void Planet::calcAccuratePosition()
{
	glPushMatrix();
	glLoadMatrixf(modelMatrix);
	glRotatef(planeAngles[0], 1.0, 0.0, 0.0);
	glRotatef(planeAngles[1], 0.0, 1.0, 0.0);
	glRotatef(planeAngles[2], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, transformedMatrix);
	glPopMatrix();

	glPushMatrix();
	glLoadMatrixf(transformedMatrix);
	glTranslatef(position[0], 0.0, position[1]);
	glGetFloatv(GL_MODELVIEW_MATRIX, transformedMatrix);
	glPopMatrix();

	for (auto &p : children)
	{
		p->setModelMatrix(transformedMatrix);
		p->calcAccuratePosition();
	}
}

ostream & operator<< (ostream &os, const Planet &p)
{
	os << "minorAxis: " << p.minorAxis << ", majorAxis: " << p.majorAxis << ", radius: " << p.radius << 
		"\narea: " << p.area << ", angle: " << p.angle << ", ellipseStep: " << p.ellipseStep;
	os << "\nangles: ";
	for (int i = 0; i < 3; i++)
		os << p.angles[i] << " ";

	os << "\nspinSpeed: ";
	for (int i = 0; i < 3; i++)
		os << p.spinSpeed[i] << " ";

	os << "\nplaneAngles: ";
	for (int i = 0; i < 3; i++)
		os << p.planeAngles[i] << " ";

	os << "\ncolor: ";
	for (int i = 0; i < 3; i++)
		os << p.color[i] << " ";

	os << "\nposition: ";
	for (int i = 0; i < 2; i++)
		os << p.position[i] << " ";
	os << "\n\n";
	for (auto p : p.children)
		os << p;

	return os;
}

// PRIVATE
void Planet::clone(const Planet &p)
{
	minorAxis = p.minorAxis;
	majorAxis = p.majorAxis;
	radius = p.radius;
	area = p.area;
	angle = p.angle;
	ellipseStep = p.ellipseStep;
	this->p = p.p;
	e = p.e;
	name = p.name;
	for (int i = 0; i < 3; i++)
	{
		angles[i] = p.angles[i];
		spinSpeed[i] = p.spinSpeed[i];
		planeAngles[i] = p.planeAngles[i];
		color[i] = p.color[i];
		if (i < 2)
			position[i] = p.position[i];
	}

	for (int i = 0; i < 16; i++)
	{
		modelMatrix[i] = p.modelMatrix[i];
		transformedMatrix[i] = p.transformedMatrix[i];
	}

	children = p.children;
}

void Planet::prepareMatrix(const bool &path)
{
	rotatePlane();
	if (path)
		drawPath();
	translate();
	drawChildren(path);
	rotate();
}

void Planet::translate()
{
	glTranslatef(position[0], 0.0, position[1]);

	glPushMatrix();
	glLoadMatrixf(transformedMatrix);
	glTranslatef(position[0], 0.0, position[1]);
	glGetFloatv(GL_MODELVIEW_MATRIX, transformedMatrix);
	glPopMatrix();

	//printf("%s:\nx=%f, y=%f, z=%f\n", name.c_str(), transformedMatrix[12], transformedMatrix[13], transformedMatrix[14]);
	//printf("%s:\nx=%f, y=%f, z=%f\n\n", name.c_str(), position[0], 0.0, position[1]);
}

void Planet::rotate()
{
	glRotatef(angles[0], 1.0, 0.0, 0.0);
	glRotatef(angles[1], 0.0, 1.0, 0.0);
	glRotatef(angles[2], 0.0, 0.0, 1.0);
}

void Planet::rotatePlane()
{
	glRotatef(planeAngles[0], 1.0, 0.0, 0.0);
	glRotatef(planeAngles[1], 0.0, 1.0, 0.0);
	glRotatef(planeAngles[2], 0.0, 0.0, 1.0);

	glPushMatrix();
	glLoadMatrixf(modelMatrix);
	glRotatef(planeAngles[0], 1.0, 0.0, 0.0);
	glRotatef(planeAngles[1], 0.0, 1.0, 0.0);
	glRotatef(planeAngles[2], 0.0, 0.0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, transformedMatrix);
	glPopMatrix();
}

void Planet::drawPath()
{
	if (majorAxis != 0.0 && minorAxis != 0.0)
	{
		point2 pos;
		float r;

		for (int eps = 1; eps <= 360; eps++)
		{
			float eps1 = (eps - 1) * M_PI / 180.0, eps2 = eps * M_PI / 180.0;
			glBegin(GL_LINES);

			r = _r(p, e, eps - 1);
			pos[0] = r * cos(eps1);
			pos[1] = r * sin(eps1);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3d(pos[0], 0.0, pos[1]);
			//printf("x = %g, y = %g\n", pos[0], pos[1], eps1);

			r = _r(p, e, eps);
			pos[0] = r * cos(eps2);
			pos[1] = r * sin(eps2);
			glColor3f(1.0, 1.0, 1.0);
			glVertex3d(pos[0], 0.0, pos[1]);
			//printf("x = %g, y = %g\n\n", pos[0], pos[1]);

			glEnd();
		}
	}
}

void Planet::drawChildren(const bool &path)
{
	for (auto &p : children)
	{
		p->setModelMatrix(transformedMatrix);
		p->draw(path);
	}
}

float Planet::_r(float p, float e, float degreesAngle)
{
	return (p / (1.0 + e * cos(degreesAngle * M_PI / 180.0)));
}