/*************************************************************************************/
//  Program symuluj¹cy ruch planet w przestrzeni 3D
/*************************************************************************************/

#include "stdafx.h"
#include "Planet.h"

using namespace std;
float dimensions = 250.0f;

clock_t animationTime;
angle3 planeAngles = { 0.0, 0.0, 0.0 };

angle3 solRotation = { 90.0, 0.0, 0.05 }; 
angle3 solPlane = { 90.0, 0.0, 0.0 };
color3 solColor = { 0.8, 0.8, 0.0 };

angle3 earthRotation = { 120.0, 30.0, 0.05 };
angle3 earthPlane = { 0.0, 0.0, 0.0 };
color3 earthColor = { 0.0, 0.0, 0.8 };

angle3 moonRotation = { 90.0, 0.0, 0.05 };
angle3 moonPlane = { 0.0, 0.0, 15.0 };
color3 moonColor = { 0.8, 0.8, 0.8 };

angle3 marsRotation = { 120.0, 30.0, 0.05 };
angle3 marsPlane = { 0.0, 120.0, 30.0 };
color3 marsColor = { 0.8, 0.0, 0.0 };

Planet sol(0.0, 0.0, 0.0, solRotation);
Planet earth(80.0, 90.0, 100.0, earthRotation);
Planet moon(20.0, 20.0, 150.0, moonRotation);
Planet mars(100.0, 150.0, 70.0, marsRotation);

bool animate = true, drawPath = true;
double frameDurationSec = 1.0 / 60.0;

// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba
// przerysowaæ scenê)
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

	// Obracanie p³aszczyzny uk³adu
	glRotatef(planeAngles[0], 1.0, 0.0, 0.0);
	glRotatef(planeAngles[1], 0.0, 1.0, 0.0);
	glRotatef(planeAngles[2], 0.0, 0.0, 1.0);

	// S³oñce START
	glPushMatrix();
	sol.prepareMatrix(drawPath);
	glColor3fv(solColor);
	glutWireSphere(15.0, 20, 16);
	glPopMatrix();
	// S³oñce KONIEC

	// Ziemia START
	glPushMatrix();
	earth.rotatePlane();
	if (drawPath)
		earth.drawPath();
	earth.translate();

	// Ksiê¿yc START
	glPushMatrix();
	moon.prepareMatrix(drawPath);
	glColor3fv(moonColor);
	glutWireSphere(2.0, 10, 8);
	glPopMatrix();
	// Ksiê¿yc KONIEC

	earth.rotate();
	glColor3fv(earthColor);
	glutWireSphere(10.0, 20, 16);
	glPopMatrix();
	// Ziemia KONIEC
	
	// Mars START
	glPushMatrix();
	mars.prepareMatrix(drawPath);
	glColor3fv(marsColor);
	glutWireSphere(10.0, 20, 16);
	glPopMatrix();
	// Mars KONIEC

	glutSwapBuffers();
}

/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	earth.setPlane(earthPlane);
	moon.setPlane(moonPlane);
	mars.setPlane(marsPlane);
	animationTime = clock();
}

/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	if (horizontal <= vertical)
		glOrtho(-dimensions, dimensions, -dimensions / AspectRatio, dimensions / AspectRatio, dimensions, -dimensions);

	else
		glOrtho(-dimensions*AspectRatio, dimensions*AspectRatio, -dimensions, dimensions, dimensions, -dimensions);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	const float angleStep = 10.0;
	const float fpsChange = 0.01f;
	int number = -1;
	bool add;

	switch (key)
	{
	case ' ':
		animate = !animate;
		break;
	case 'w':
		add = true;
		number = 0;
		break;
	case 's':
		add = false;
		number = 0;
		break;
	case 'a':
		add = false;
		number = 1;
		break;
	case 'd':
		add = true;
		number = 1;
		break;
	case 'q':
		add = true;
		number = 2;
		break;
	case 'e':
		add = false;
		number = 2;
		break;
	case 'r':
		planeAngles[0] = planeAngles[1] = planeAngles[2] = 0.0;
		break;
	case 'p':
		drawPath = !drawPath;
		break;
	default:
		number = -1;
		break;
	}

	// Zmiana nachylenia p³aszczyzny w stosunku do obserwatora
	if (number >= 0 && number < 3)
	{
		if (add)
		{
			if (planeAngles[number] < 360.0)
				planeAngles[number] += angleStep;
			else
				planeAngles[number] = 0.0;
		}
		else
		{
			if (planeAngles[number] > angleStep)
				planeAngles[number] -= angleStep;
			else
				planeAngles[number] = 360.0;
		}
		printf("Plane angles - x = %g, y = %g, z = %g\n", planeAngles[0], planeAngles[1], planeAngles[2]);
	}

	glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}

void animation()
{
	double timeInSec = (double(clock()) - double(animationTime)) / double(CLOCKS_PER_SEC);
	const GLfloat step = 0.1f;
	
	if (animate)
	{
		if (timeInSec > frameDurationSec)
		{
			sol.nextStep();
			earth.nextStep();
			moon.nextStep();
			mars.nextStep();
			animationTime = clock();
			glutPostRedisplay();
		}
	}
}

/*************************************************************************************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
void main(void)
{
	stringstream title;
	title << "Uk³ad s³oneczny";
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow(title.str().c_str());

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(animation);

	glutKeyboardFunc(keyboard);

	MyInit();

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

}
/*************************************************************************************/