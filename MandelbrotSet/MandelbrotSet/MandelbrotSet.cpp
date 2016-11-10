// MandelbrotSet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
const int convergenceSteps = 50;
const double divergenceLimit = 2.0;
double xMin = -(divergenceLimit + 0.5);
double xMax = (divergenceLimit + 0.5);
double yMin = -(divergenceLimit + 0.5);
double yMax = (divergenceLimit + 0.5);
double resolution = 0.01;
double zoom = 2.0;
bool isColor = true;

void magnify(int x, int y, double magnitude)
{
	if (magnitude <= 0.0)
		return;

	double size = (xMax - xMin);
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	double windowSize;

	if (!height)
		height = 1;
	if (!width)
		width = 1;

	if (height < width)
	{
		x -= ((width - height) / 2);
		windowSize = height;
	}
	else
	{
		y -= ((height - width) / 2);
		windowSize = width;
	}

	y = windowSize - y;
	x -= windowSize / 2.0;
	y -= windowSize / 2.0;

	double xCenter = (((double)x / windowSize) * size) + ((xMax + xMin) / 2.0);
	double yCenter = (((double)y / windowSize) * size) + ((yMax + yMin) / 2.0);
	double difference = (size * magnitude) / 2.0;

	xMin = xCenter - difference;
	xMax = xCenter + difference;
	yMin = yCenter - difference;
	yMax = yCenter + difference;
}

void RenderScene(void)
{
	cout << "Entering RenderScene" << endl;
	cout << "x=(" << xMin << ", " << xMax << ")" << endl;
	cout << "y=(" << yMin << ", " << yMax << ")" << endl;
	cout << "resolution=" << resolution << endl;
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	unsigned long long iterations = 0;
	double timePerIteration = 0.0, overallTime = 0.0;
	clock_t start = clock();
	
	for (double x = xMin; x < xMax; x += resolution)
	{
		for (double y = yMin; y < yMax; y += resolution)
		{
			complex<double> z(0, 0);
			complex<double> c(x, y);
			// Sprawdzanie zbieznosci
			int j = 0;
			clock_t time;
			for (; j < convergenceSteps && abs(z) < divergenceLimit; j++)
			{
				++iterations;
				time = clock();
				z = z * z + c;
				timePerIteration += ((clock() - time) / (double)CLOCKS_PER_SEC) * 1000000000.0;
			}

			double color = (double)j / (double)convergenceSteps;
			glBegin(GL_POINTS);

			if (isColor)
			{
				int maxColor = 0xffffff;
				int clr = floor(color * (double)maxColor);
				int r, g, b;
				r = (clr & (0xff << 16)) >> 16;
				g = (clr & (0xff << 8)) >> 8;
				b = clr & (0xff);
				glColor3b(r, g, b);
			}
			else
			{
				glColor3d(color, color, color);
			}

			glVertex2d(x, y);
			glEnd();
		}
	}

	glFlush();
	start = clock() - start;
	overallTime = start / (double)CLOCKS_PER_SEC;
	timePerIteration /= (double)iterations;
	// Przekazanie poleceñ rysuj¹cych do wykonania
	cout << "Exiting RenderScene after " <<overallTime<<" s, "<< iterations << " iterations, \ntime per iteration: " 
		<< timePerIteration	<<" ns"<< endl << endl;
}

void MyInit(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Kolor okna wnêtrza okna - ustawiono na bia³y
	cout << "\"+, -\" - zmiana rozdzielczosci rysowania\n"
		"\"z, a\" - zwiekszanie/zmniejszanie powiekszenia\n"
		"LMB, RMB - powiekszenie/pomniejszenie\n"
		"\"r, c\" - wymuszenie przerysowania/zmiany rozmiaru\n"
		"\"h\" - ten tekst pomocy\n"<< endl;
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna
{
	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glMatrixMode(GL_PROJECTION);
	// Okreœlenie uk³adu wspó³rzêdnych obserwatora

	glLoadIdentity();
	// Okreœlenie przestrzeni ograniczaj¹cej

	if (horizontal <= vertical)
	{
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
		glOrtho(xMin, xMax, yMin, yMax, 1.0, -1.0);
		resolution = (yMax - yMin) / (double)horizontal;
	}
	else
	{
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
		glOrtho(xMin, xMax, yMin, yMax, 1.0, -1.0);
		resolution = (xMax - xMin) / (double)vertical;
	}

	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych    

	glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
	static int leftX, leftY, rightX, rightY;
	double xSize = xMax - xMin;
	double ySize = yMax - yMin;
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	int width = glutGet(GLUT_WINDOW_WIDTH);

	int lowerSize = height, greaterSize = width, axis = x;
	if (height > width)
	{
		lowerSize = width;
		greaterSize = height;
		axis = y;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		leftX = x;
		leftY = y;
		cout << "LEFT x: " << leftX << ", y: " << leftY << endl;

		if (axis < ((greaterSize - lowerSize) / 2) || axis >((greaterSize + lowerSize) / 2))
		{
			cout << "Click out of picture" << endl;
			leftX = -1;
			leftY = -1;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		rightX = x;
		rightY = y;
		cout << "RIGHT x: " << rightX << ", y: " << rightY << endl;

		if (axis < ((greaterSize - lowerSize) / 2) || axis >((greaterSize + lowerSize) / 2))
		{
			cout << "Click out of picture" << endl;
			rightX = -1;
			rightY = -1;
		}
	}
	else if (state == GLUT_UP) {

		if (button == GLUT_LEFT_BUTTON && leftX >= 0 && leftY >= 0)
		{
			cout << "Magnify: " << zoom << endl;
			magnify(leftX, leftY, 1/zoom);
			ChangeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			RenderScene();
		}
		else if (button == GLUT_RIGHT_BUTTON && rightX >= 0 && rightY >= 0)
		{
			cout << "Magnify: " << 1/zoom << endl;
			magnify(rightX, rightY, zoom);
			ChangeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			RenderScene();
		}
	}
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'r')
	{
		RenderScene();
	}
	else if (key == 'c')
	{
		isColor = !isColor;
		RenderScene();
	}
	else if (key == '+')
	{
		resolution *= 0.9;
		RenderScene();
	}
	else if (key == '-')
	{
		resolution *= 1.1;
		RenderScene();
	}
	else if (key == 'a')
	{
		if (zoom > 1.0)
		{
			zoom -= 1.0;
			cout << "Changed zoom to " << zoom << endl;
		}
		else
			zoom = 1.0;
	}
	else if (key == 'z')
	{
		if (zoom < 20.0)
		{
			zoom += 1.0;
			cout << "Changed zoom to " << zoom << endl;
		}
		else
			zoom = 20.0;
	}
	else if (key == 'h')
	{
		cout << "\n\"+, -\" - zmiana rozdzielczosci rysowania\n"
			"\"z, a\" - zwiekszanie/zmniejszanie powiekszenia\n"
			"LMB, RMB - powiekszenie/pomniejszenie\n"
			"\"r, c\" - wymuszenie przerysowania/czy kolorowac\n"
			"\"h\" - ten tekst pomocy\n" << endl;
	}
}

void main(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyœwietlania
	// GLUT_RGBA - model kolorów RGB

	glutCreateWindow("Mandelbrot Set");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna

	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);

	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/