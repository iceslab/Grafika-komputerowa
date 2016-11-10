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

	//y = windowSize - y;
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

void calcMandelbrot(int* &mandelbrot, int width, int height, point2int vecX, point2int vecY)
{
	for (int k = vecY[0]; k < vecY[1]; k++)
	{
		double y = yMax - (yMax - yMin) * (double(k) / double(height));
		for (int i = vecX[0]; i < vecX[1]; i++)
		{
			double x = xMin + (xMax - xMin) * (double(i) / double(width));
			
			complex<double> z(0, 0);
			complex<double> c(x, y);
			// Sprawdzanie zbieznosci
			int j = 0;
			for (; j < convergenceSteps && abs(z) < divergenceLimit; j++)
			{
				z = z * z + c;
			}
			mandelbrot[k * width + i] = j;
		}
	}
}

void RenderScene(void)
{
	cout << "Entering RenderScene++" << endl;
	cout << "x=(" << xMin << ", " << xMax << ")" << endl;
	cout << "y=(" << yMin << ", " << yMax << ")" << endl;
	cout << "resolution=" << resolution << endl;
	
	// Clear the model-view matrix
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	unsigned long long iterations = 0;
	double timePerIteration = 0.0, overallTime = 0.0;
	clock_t start = clock();
	int width, height;
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	glPushMatrix();
	// Clear the model-view matrix
	glLoadIdentity();

	if (width <= height)
	{
		height = width;
	}
	else
	{
		width = height;
	}

	int *mandelbrot = (int*)malloc((height * width) * sizeof(int));
	
	point2int vecX[4] = { { 0, width / 4 },
	{ width / 4, width / 2 },
	{ width / 2, 3 * width / 4 },
	{ 3 * width / 4, width } };
	
	point2int vecY[4] = { { 0, height},
	{ 0, height },
	{ 0, height },
	{ 0, height } };

	thread t0(calcMandelbrot, mandelbrot, width, height, vecX[0], vecY[0]);
	thread t1(calcMandelbrot, mandelbrot, width, height, vecX[1], vecY[1]);
	thread t2(calcMandelbrot, mandelbrot, width, height, vecX[2], vecY[2]);
	thread t3(calcMandelbrot, mandelbrot, width, height, vecX[3], vecY[3]);
	
	t0.join();
	t1.join();
	t2.join();
	t3.join();

	glBegin(GL_POINTS);
	for (int i = 0; i < width; i++)
	{
		double x = xMin + (xMax - xMin) * (double(i) / double(width));
		for (int k = 0; k < height; k++)
		{
			double y = yMin + (yMax - yMin) * (double(k) / double(height));

			double color = (double)mandelbrot[i * width + k] / (double)convergenceSteps;
			int maxColor = 0xffffff;
			int clr = floor(color * (double)maxColor);
			int r, g, b;
			r = (clr & (0xff << 16)) >> 16;
			g = (clr & (0xff << 8)) >> 8;
			b = clr & (0xff);

			glColor3b(r, g, b);
			glVertex2i(k, i);
		}
	}
	glEnd();

	glFlush();
	start = clock() - start;
	overallTime = start / (double)CLOCKS_PER_SEC;
	timePerIteration /= (double)iterations;
	// Przekazanie polece� rysuj�cych do wykonania
	cout << "Exiting RenderScene after " <<overallTime<<" s, "<< iterations << " iterations, \ntime per iteration: " 
		<< timePerIteration	<<" ns"<< endl << endl;
}

void MyInit(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Kolor okna wn�trza okna - ustawiono na bia�y
	cout << "\"+, -\" - zmiana rozdzielczosci rysowania\n"
		"\"z, a\" - zwiekszanie/zmniejszanie powiekszenia\n"
		"LMB, RMB - powiekszenie/pomniejszenie\n"
		"\"r, c\" - wymuszenie przerysowania/zmiany rozmiaru\n"
		"\"h\" - ten tekst pomocy\n"<< endl;
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna
{
	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glMatrixMode(GL_PROJECTION);
	// Okre�lenie uk�adu wsp�rz�dnych obserwatora

	glLoadIdentity();
	// Okre�lenie przestrzeni ograniczaj�cej

	if (horizontal <= vertical)
	{
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
		glOrtho(0, horizontal, 0, horizontal, 1.0, -1.0);
	}
	else
	{
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
		glOrtho(0, vertical, 0, vertical, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych    

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
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB

	glutCreateWindow("Mandelbrot Set++");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna

	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);

	MyInit();
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst�pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/