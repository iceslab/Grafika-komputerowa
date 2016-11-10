#pragma once
#include "stdafx.h"
#include "Planet.h"

array<char*, 10> textureNames =
{
	"sol.tga",
	"mercury.tga",
	"venus.tga",
	"earth.tga",
	"moon.tga",
	"mars.tga",
	"jupiter.tga",
	"saturn.tga",
	"uranus.tga",
	"neptune.tga"
};

static angle3 solRotation = { 90.0, 0.0, 0.5 };
static angle3 mercuryRotation = { 120.0, 30.0, 0.5 };
static angle3 venusRotation = { 120.0, 30.0, 0.5 };
static angle3 earthRotation = { 120.0, 30.0, 1.5 };
static angle3 moonRotation = { 90.0, 0.0, 1.5 };
static angle3 marsRotation = { 120.0, 30.0, 1.5 };
static angle3 jupiterRotation = { 120.0, 30.0, 2.5 };
static angle3 saturnRotation = { 120.0, 30.0, 2.0 };
static angle3 uranusRotation = { 0.0, 30.0, 2.0 };
static angle3 neptuneRotation = { 120.0, 30.0, 2.5 };

Planet sol(0.0, 0.0, 20.0, 0.0, solRotation);
Planet mercury(60.0, 70.0, 5.0, 100.0, mercuryRotation);
Planet venus(100.0, 90.0, 6.0, 50.0, venusRotation);
Planet earth(130.0, 180.0, 7.0, 60.0, earthRotation);
Planet moon(20.0, 20.0, 4.0, 100.0, moonRotation);
Planet mars(220.0, 250.0, 6.0, 40.0, marsRotation);
Planet jupiter(400.0, 500.0, 12.0, 30.0, jupiterRotation);
Planet saturn(650.0, 770.0, 10.0, 20.0, saturnRotation);
Planet uranus(830.0, 890.0, 9.0, 10.0, uranusRotation);
Planet neptune(1000.0, 1200.0, 9.0, 10.0, neptuneRotation);

inline void initPlanets()
{
	sol.setName("Sol");
	sol.setPlane(0.0, 0.0, 0.0);
	sol.setColor(0.8, 0.8, 0.0);

	mercury.setName("Mercury");
	mercury.setPlane(10.0, 15.0, 10.0);
	mercury.setColor(1.0, 0.5, 0.1);

	venus.setName("Venus");
	venus.setPlane(0.0, 43.0, 10.0);
	venus.setColor(0.65, 0.5, 0.0);

	earth.setName("Earth");
	earth.setPlane(0.0, 0.0, 0.0);
	earth.setColor(0.0, 0.0, 0.8);

	moon.setName("Moon");
	moon.setPlane(0.0, 0.0, 15.0);
	moon.setColor(0.8, 0.8, 0.8);

	mars.setName("Mars");
	mars.setPlane(0.0, 120.0, 30.0);
	mars.setColor(0.8, 0.0, 0.0);

	jupiter.setName("Jupiter");
	jupiter.setPlane(22.0, 84.0, 10.0);
	jupiter.setColor(1.0, 0.75, 0.3);

	saturn.setName("Saturn");
	saturn.setPlane(40.0, 67.0, 20.0);
	saturn.setColor(1.0, 0.85, 0.4);

	uranus.setName("Uranus");
	uranus.setPlane(6.0, 200.0, 15.0);
	uranus.setColor(0.85, 0.95, 1.0);

	neptune.setName("Neptune");
	neptune.setPlane(30.0, 160.0, 30.0);
	neptune.setColor(0.0, 0.55, 0.92);

	earth.addChild(moon);
	sol.addChild(mercury);
	sol.addChild(venus);
	sol.addChild(earth);
	sol.addChild(mars);
	sol.addChild(jupiter);
	sol.addChild(saturn);
	sol.addChild(uranus);
	sol.addChild(neptune);
}

inline void initTextures(GLuint* textureId)
{
	sol.setTextureId(textureId[0]);
	mercury.setTextureId(textureId[1]);
	venus.setTextureId(textureId[2]);
	earth.setTextureId(textureId[3]);
	moon.setTextureId(textureId[4]);
	mars.setTextureId(textureId[5]);
	jupiter.setTextureId(textureId[6]);
	saturn.setTextureId(textureId[7]);
	uranus.setTextureId(textureId[8]);
	neptune.setTextureId(textureId[9]);
}