#pragma once
#include "stdafx.h"

class Planet
{
private:
	float minorAxis, majorAxis;
	float area, p, e, angle, ellipseStep;
	angle3 angles, spinSpeed, planeAngles;
	point2 position;

public:
	Planet(float firstAxis, float secondAxis, float startingAngleVelocity, angle3 spinSpeed, float startingAngle = 0.0f)
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

		float r = p / (1.0 + e * cos(ellipseStep * M_PI / 180.0));
		area = (pow(r, 2.0) * (ellipseStep * M_PI / 180.0)) / 2.0;
	}

	void nextStep()
	{
		if (majorAxis != 0.0 && minorAxis != 0.0)
		{
			float r = p / (1.0 + e * cos(angle * M_PI / 180.0));
			angle += ellipseStep;
			while (angle >= 360.0)
				angle -= 360.0;

			ellipseStep = (area * 2.0) / pow(r, 2.0);

			position[0] = r * cos(angle * M_PI / 180.0);
			position[1] = r * sin(angle * M_PI / 180.0);
		}

		angles[2] += spinSpeed[2];
		while (angles[2] >= 360.0)
			angles[2] -= 360.0;
	}

	void prepareMatrix(bool path = false)
	{
		rotatePlane();
		if (path)
			drawPath();
		translate();
		rotate();
	}

	void setPlane(angle3 angles)
	{
		planeAngles[0] = angles[0];
		planeAngles[1] = angles[1];
		planeAngles[2] = angles[2];
	}

	void translate()
	{
		glTranslatef(position[0], 0.0, position[1]);
	}

	void rotate()
	{
		glRotatef(angles[0], 1.0, 0.0, 0.0);
		glRotatef(angles[1], 0.0, 1.0, 0.0);
		glRotatef(angles[2], 0.0, 0.0, 1.0);
	}

	void rotatePlane()
	{
		glRotatef(planeAngles[0], 1.0, 0.0, 0.0);
		glRotatef(planeAngles[1], 0.0, 1.0, 0.0);
		glRotatef(planeAngles[2], 0.0, 0.0, 1.0);
	}

	void drawPath()
	{
		if (majorAxis != 0.0 && minorAxis != 0.0)
		{
			point2 pos;
			float r;

			for (int eps = 1; eps <= 360; eps++)
			{
				float eps1 = (eps - 1) * M_PI / 180.0, eps2 = eps * M_PI / 180.0;
				glBegin(GL_LINES);

				r = p / (1.0 + e * cos(eps1));
				pos[0] = r * cos(eps1);
				pos[1] = r * sin(eps1);
				glColor3f(1.0, 1.0, 1.0);
				glVertex3d(pos[0], 0.0, pos[1]);
				//printf("x = %g, y = %g\n", pos[0], pos[1]);

				r = p / (1.0 + e * cos(eps2));
				pos[0] = r * cos(eps2);
				pos[1] = r * sin(eps2);
				glColor3f(1.0, 1.0, 1.0);
				glVertex3d(pos[0], 0.0, pos[1]);
				//printf("x = %g, y = %g\n\n", pos[0], pos[1]);

				glEnd();
			}
		}
	}
};