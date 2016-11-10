#pragma once

#define _USE_MATH_DEFINES
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>
#include <random>
#include <ctime>
#include <sstream>

using namespace std;
typedef float point3[3];
typedef GLubyte color3[3];
typedef int array3[3];
GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k¹ty obrotu
const int N = 80;
point3 eggPoints[N][N];
point3 eggNormalU[N][N];
point3 eggNormalV[N][N];
color3 colors[N][N];
bool calculated = false;
bool spin = false;
int model = GL_POINTS;
clock_t animationTime;
const float startingFps = 0.01f;
double frameDurationSec = startingFps;
double R = 10.0;

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja po³o¿enia obserwatora

//static GLfloat theta[2] = { 0.0, 0.0 };   // k¹t obrotu obiektu
static GLfloat pix2angleH, pix2angleV;     // przelicznik pikseli na stopnie
static GLint status = 0;       // stan klawiszy myszy
// 0 - nie naciœniêto ¿adnego klawisza
// 1 - naciœniêty zosta³ lewy klawisz
// 2 - naciœniêty zosta³ prawy klawisz

static int x_pos_old = 0, y_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0, delta_y = 0.01;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
// i poprzedni¹ kursora myszy 