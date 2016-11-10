// MandelbrotSet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuComplex.h"
#include <math.h>
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

__global__ void addKernel(int* d_mandelbrot, int sizeI, int sizeK, point2d xBound, point2d yBound, int con, double div)
{
	int k = threadIdx.x;

	double y = yBound[0] + (yBound[1] - yBound[0]) * (double(k) / double(sizeK));
	
		for (int i = 0; i < sizeI; ++i)
		{
			double x = xBound[0] + (xBound[1] - xBound[0]) * (double(i) / double(sizeI));
			
			cuDoubleComplex z = make_cuDoubleComplex(0.0, 0.0);
			cuDoubleComplex c = make_cuDoubleComplex(x, y);
			// Sprawdzanie zbieznosci
			int j = 0;
			cuCabs(z);
			for (; j < con && cuCabs(z) < div; j++)
			{
				z = cuCadd(cuCmul(z, z), c);
			}

			/*point2d z = {0.0, 0.0};
			point2d c = {x, y};
			// Sprawdzanie zbieznosci
			int j = 0;
			
			
			for (; (j < con) && (sqrt(z[0] * z[0] + z[1] * z[1]) < div); j++)
			{
				z[0] = z[0] * z[0] - z[1] * z[1];
				z[1] = z[1] * z[0] + z[0] * z[1];
				z[0] += c[0];
				z[1] += c[1];
			}*/
			d_mandelbrot[(i * sizeI) + k] = j;
		}
}

void RenderScene(void)
{
	cout << "Entering RenderScene++" << endl;
	cout << "x=(" << xMin << ", " << xMax << ")" << endl;
	cout << "y=(" << yMin << ", " << yMax << ")" << endl;
	cout << "resolution=" << resolution << endl;
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	unsigned long long iterations = 0;
	double timePerIteration = 0.0, overallTime = 0.0;
	clock_t start = clock();
	int width, height;
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	if (width <= height)
	{
		height = width;
		//glViewport(0, (height - width) / 2, width, width);
	}
	else
	{
		width = height;
		//glViewport((width - height) / 2, 0, height, height);
	}

	int *mandelbrot = (int*)malloc((height * width) * sizeof(int));
	int *d_mandelbrot = 0;
	point2d xBound = { xMin, xMax };
	point2d yBound = { yMin, yMax };
	double* d_xBound = 0;
	double* d_yBound = 0;
	cudaError_t cudaStatus;

	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}

	// Allocate GPU buffers for three vectors (two input, one output)    .
	cudaStatus = cudaMalloc((void**)&d_mandelbrot, (height * width) * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	// Allocate GPU buffers for three vectors (two input, one output)    .
	cudaStatus = cudaMalloc((void**)&d_xBound, 2 * sizeof(double));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}
	// Allocate GPU buffers for three vectors (two input, one output)    .
	cudaStatus = cudaMalloc((void**)&d_yBound, 2 * sizeof(double));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}
	
	cudaStatus = cudaMemcpy(d_xBound, xBound, 2 * sizeof(double), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}
	cudaStatus = cudaMemcpy(d_yBound, yBound, 2 * sizeof(double), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}

	// Launch a kernel on the GPU with one thread for each element.
	addKernel << <1, height >> >(d_mandelbrot, width, height, d_xBound, d_yBound, convergenceSteps, divergenceLimit);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		goto Error;
	}

	cudaStatus = cudaMemcpy(mandelbrot, d_mandelbrot, (height * width) * sizeof(int), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		fprintf(stderr, cudaGetErrorString(cudaGetLastError()));
		goto Error;
	}
	cudaFree(d_mandelbrot);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		fprintf(stderr, cudaGetErrorString(cudaGetLastError()));
		goto Error;
	}

	for (int i = 0; i < width; i++)
	{
		for (int k = 0; k < height; k++)
		{
			double color = (double)mandelbrot[i * height + k] / (double)convergenceSteps;
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

			glVertex2i(i, k);
			glEnd();
		}
	}

	glFlush();
	free(mandelbrot);
	start = clock() - start;
	overallTime = start / (double)CLOCKS_PER_SEC;
	timePerIteration /= (double)iterations;
	// Przekazanie poleceñ rysuj¹cych do wykonania
	cout << "Exiting RenderScene after " << overallTime << " s, " << iterations << " iterations, \ntime per iteration: "
		<< timePerIteration << " ns" << endl << endl;
	return;
Error:
	cudaFree(d_mandelbrot);
}

void MyInit(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Kolor okna wnêtrza okna - ustawiono na bia³y
	cout << "\"+, -\" - zmiana rozdzielczosci rysowania\n"
		"\"z, a\" - zwiekszanie/zmniejszanie powiekszenia\n"
		"LMB, RMB - powiekszenie/pomniejszenie\n"
		"\"r, c\" - wymuszenie przerysowania/zmiany rozmiaru\n"
		"\"h\" - ten tekst pomocy\n" << endl;
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
		glOrtho(0, horizontal, 0, horizontal, 1.0, -1.0);
	}
	else
	{
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
		glOrtho(0, vertical, 0, vertical, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych    

	glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
	static int leftX, leftY, rightX, rightY;
	//double xSize = xMax - xMin;
	//double ySize = yMax - yMin;
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
			magnify(leftX, leftY, 1 / zoom);
			ChangeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			RenderScene();
		}
		else if (button == GLUT_RIGHT_BUTTON && rightX >= 0 && rightY >= 0)
		{
			cout << "Magnify: " << 1 / zoom << endl;
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

int main(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyœwietlania
	// GLUT_RGBA - model kolorów RGB

	glutCreateWindow("Mandelbrot Set CUDA");
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