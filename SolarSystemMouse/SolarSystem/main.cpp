/*************************************************************************************/
//  Program symuluj¹cy ruch planet w przestrzeni 3D
/*************************************************************************************/
#include "solarSystemSetup.h"

using namespace std;

inline float calcX();
inline float calcY();
inline float calcZ();

function<float(void)> calc[3] = { calcX, calcY, calcZ };

inline void calcOrthonormal(point2 start, point2 end, point2 &dest)
{

	float srcX = end[0] - start[0], srcY = end[1] - start[1];
	if (srcX == 0.0f && srcY == 0.0f)
	{
		dest[0] = dest[1] = 0.0f;
	}
	else if (srcX == 0.0f)
	{
		dest[1] = 0.0f;
		if (srcY > 0.0f)
			dest[0] = 1.0f;
		else
			dest[0] = -1.0f;
	}
	else if (srcY == 0.0f)
	{
		dest[0] = 0.0f;
		if (srcX > 0.0f)
			dest[1] = -1.0f;
		else
			dest[1] = 1.0f;
	}
	else
	{
		dest[0] = srcY / (sqrt(pow(srcX, 2.0) + pow(srcY, 2.0)));
		dest[1] = -((srcX * dest[0]) / srcY);
	}
}

inline float calcX()
{
	return R * cos(theta[0] * factor) * cos(theta[1] * factor);
}

inline float calcY()
{
	return R * sin(theta[1] * factor);
}

inline float calcZ()
{
	return R * sin(theta[0] * factor) * cos(theta[1] * factor);
}
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba
// przerysowaæ scenê)
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

	if (status)
	{
		if (status == -1)
			status = 0;
		else if (status == 1)	// jeœli lewy klawisz myszy wciœniêty
		{
			GLfloat diff = delta_y*pix2angleV;
			theta[0] += delta_x*pix2angleH;    // modyfikacja k¹ta obrotu o kat proporcjonalny
			if (theta[1] + diff > -90.0 && theta[1] + diff < 90.0)
				theta[1] += diff;    // modyfikacja k¹ta obrotu o kat proporcjonalny
		}
		else if (status == 2)
		{
			GLfloat diff = delta_y*pix2angleV;
			if (R + diff > 0.0)
				R += diff;
		}
	}
	
	sol.calcAccuratePosition();
	switch (currentCenter)
	{
	default:
		break;
	case NONE:
		viewer[0] = viewerCenter[0] + calcX();
		viewer[1] = viewerCenter[1] + calcY();
		viewer[2] = viewerCenter[2] + calcZ();
		break;
	case SOL:
		sol.getWorldPosition(viewer);
		R = sol.getRadius();
		break;
	case MERCURY:
		mercury.getWorldPosition(viewer);
		R = mercury.getRadius();
		break;
	case VENUS:
		venus.getWorldPosition(viewer);
		R = venus.getRadius();
		break;
	case EARTH:
		earth.getWorldPosition(viewer);
		R = earth.getRadius();
		break;
	case MOON:
		moon.getWorldPosition(viewer);
		R = moon.getRadius();
		break;
	case MARS:
		mars.getWorldPosition(viewer);
		R = mars.getRadius();
		break;
	case JUPITER:
		jupiter.getWorldPosition(viewer);
		R = jupiter.getRadius();
		break;
	case SATURN:
		saturn.getWorldPosition(viewer);
		R = saturn.getRadius();
		break;
	case URANUS:
		uranus.getWorldPosition(viewer);
		R = uranus.getRadius();
		break;
	case NEPTUNE:
		neptune.getWorldPosition(viewer);
		R = neptune.getRadius();
		break;
	}

	if (currentCenter != NONE)
	{
		viewer[0] += calcX();
		viewer[1] += calcY();
		viewer[2] += calcZ();

		viewerCenter[0] = viewer[0] + 2.0 * calcX();
		viewerCenter[1] = viewer[1] + 2.0 * calcY();
		viewerCenter[2] = viewer[2] + 2.0 * calcZ();
	}
	
	//printf("x=%f, y=%f, z=%f\n\n", viewerCenter[0], viewerCenter[1], viewerCenter[2]);
	float upx = 0.0, upy = 1.0, upz = 0.0;
	
	gluLookAt
		(viewer[0], viewer[1], viewer[2],
		viewerCenter[0], viewerCenter[1], viewerCenter[2],
		upx, upy, upz);
	// Narysuj uk³ad
	sol.draw(drawPath);
	
	glutSwapBuffers();
	//glFlush();
}

/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

	sol.setModelMatrix(matrix);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	initPlanets();
}

/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	if (horizontal == 0)  // Zabezpieczenie przed dzieleniem przez 0
		horizontal = 1;
	pix2angleH = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	pix2angleV = 360.0 / (float)vertical;
	GLfloat AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji
	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej
	gluPerspective(70, AspectRatio, 1.0, dimensions);
	// Ustawienie parametrów dla rzutu perspektywicznego

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

}

void keyboard(unsigned char key, int x, int y)
{
	const float fpsChange = 0.01f;
	point2 start = { viewer[0], viewer[2] };
	point2 end = { viewerCenter[0], viewerCenter[2] };
	point2 vec;

	switch (key)
	{
	case 'w':
		if (currentCenter != NONE)
			break;
		for (int i = 0; i < 3; i++)
		{
			viewer[i] = viewerCenter[i];
			viewerCenter[i] = viewer[i] - 2.0 * calc[i]();
		}
		break;	
	case 's':
		if (currentCenter != NONE)
			break;
		for (int i = 0; i < 3; i++)
		{
			viewer[i] = viewerCenter[i];
			viewerCenter[i] = viewer[i] + 2.0 * calc[i]();
		}
		break;
	case 'W':
		if (currentCenter != NONE)
			break;
		++viewer[1];
		++viewerCenter[1];
		break;
	case 'S':
		if (currentCenter != NONE)
			break;
		--viewer[1];
		--viewerCenter[1];
		break;
	case 'a':
		if (currentCenter != NONE)
			break;
		calcOrthonormal(start, end, vec);
		viewer[0] += vec[0];
		viewer[2] += vec[1];
		viewerCenter[0] += vec[0];
		viewerCenter[2] += vec[1];
		break;
	case 'd':
		if (currentCenter != NONE)
			break;
		calcOrthonormal(start, end, vec);
		viewer[0] -= vec[0];
		viewer[2] -= vec[1];
		viewerCenter[0] -= vec[0];
		viewerCenter[2] -= vec[1];
		break;
	case ' ':
		animate = !animate;
		break;
	case 'r':
		theta[0] = theta[1] = 0.0;
		R = 10.0f;
		status = -1;
		break;
	case 'p':
		drawPath = !drawPath;
		break;
	case NONE:
		R = 1.0f;
	case SOL:
	case MERCURY:
	case VENUS:
	case EARTH:
	case MOON:
	case MARS:
	case JUPITER:
	case SATURN:
	case URANUS:
	case NEPTUNE:
		currentCenter = key;
		status = -1;
		break;
	default:
		break;
	}

	glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}

void animation()
{
	const GLfloat step = 0.1f;
	
	if (animate)
	{
		clock_t start = clock();
		sol.nextStep();
		glutPostRedisplay();
		start = clock() - start;
		double timeInMs = (double(start) * 1000.0) / double(CLOCKS_PER_SEC);
		
		if (timeInMs < frameDurationMs)
		{
			Sleep(frameDurationMs - timeInMs);
		}
	}
	else
		Sleep(frameDurationMs);
}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		// jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y_pos_old = y;
		status = 2;
	}
	else
		status = 0;          // nie zosta³ wciêniêty ¿aden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
	x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

	delta_y = y - y_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
	y_pos_old = y;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

	glutPostRedisplay();     // przerysowanie obrazu sceny
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
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	MyInit();

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

}
/*************************************************************************************/