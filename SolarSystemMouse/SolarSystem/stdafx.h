#pragma once

#define _USE_MATH_DEFINES
#include <windows.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>
#include <random>
#include <ctime>
#include <sstream>
#include <functional>

enum Planets
{
	NONE = '`',
	SOL = '1',
	MERCURY = '2',
	VENUS = '3',
	EARTH = '4', MOON = '5',
	MARS = '6',
	JUPITER = '7',
	SATURN = '8',
	URANUS = '9',
	NEPTUNE = '0'
};

typedef GLfloat point3[3];
typedef GLfloat point2[2];
typedef GLfloat color3[3];
typedef GLfloat angle3[3];
typedef GLfloat angle2[2];

const float factor = M_PI / 180.0f;
static float dimensions = 7000.0f;

static angle2 theta = { 0.0, 0.0 };
static float R = 10.0f;

static bool animate = true, drawPath = true, freeCamera = true;
static double frameDurationMs = 1000.0 / 60.0;

static point3 viewer = { 0.0, 0.0, 10.0 };
static point3 viewerCenter = { 0.0, 0.0, 0.0 };
// inicjalizacja po³o¿enia obserwatora
static int currentCenter = SOL;

static point2 lookAtPos = { 0.0, 0.0 };

static GLfloat pix2angleH, pix2angleV;     // przelicznik pikseli na stopnie
static GLint status = -1;       // stan klawiszy myszy
// -1 - przelicz na nowo pozycjê obserwatora
// 0 - nie naciœniêto ¿adnego klawisza
// 1 - naciœniêty zosta³ lewy klawisz
// 2 - naciœniêty zosta³ prawy klawisz

static int x_pos_old = 0, y_pos_old = 0;       // poprzednia pozycja kursora myszy
static int delta_x = 0, delta_y = 0.0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
// i poprzedni¹ kursora myszy 