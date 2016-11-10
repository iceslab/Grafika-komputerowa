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
GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k�ty obrotu
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
// inicjalizacja po�o�enia obserwatora

//static GLfloat theta[2] = { 0.0, 0.0 };   // k�t obrotu obiektu
static GLfloat pix2angleH, pix2angleV;     // przelicznik pikseli na stopnie
static GLint status = 0;       // stan klawiszy myszy
// 0 - nie naci�ni�to �adnego klawisza
// 1 - naci�ni�ty zosta� lewy klawisz
// 2 - naci�ni�ty zosta� prawy klawisz

static int x_pos_old = 0, y_pos_old = 0;       // poprzednia pozycja kursora myszy

static int delta_x = 0, delta_y = 0.01;        // r�nica pomi�dzy pozycj� bie��c�
// i poprzedni� kursora myszy 