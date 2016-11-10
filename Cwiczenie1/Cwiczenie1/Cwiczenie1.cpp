/*************************************************************************************/
// Troch� bardziej skomplikowany program wykorzystuj�cy funkcje biblioteki OpenGL
/*************************************************************************************/
#include "stdafx.h"
const float squareSide = 200.0f;
const float resolution = 0.02f;
point2 leftTopCorner = { -(squareSide / 2), (squareSide / 2) };
float distortion = 0.0f;
int levels = 3;

using namespace std;

float floatRand(float distort, float sideLength)
{
	float rnd = ((rand() / static_cast<float>(RAND_MAX)) * distort);
	rnd *= (-1 * (rand() % 2));
	rnd *= sideLength;
	return rnd;
}

void drawSquare(point2 leftTopCorner, float sideLength, float distort, color* colors = nullptr)
{
	point2 square[4];

	// Lewy g�rny r�g
	square[0][0] = leftTopCorner[0] + floatRand(distort, sideLength);	// x
	square[0][1] = leftTopCorner[1] + floatRand(distort, sideLength);	// y

	// Prawy g�rny r�g
	square[1][0] = square[0][0] + sideLength + floatRand(distort, sideLength);
	square[1][1] = square[0][1] + floatRand(distort, sideLength);

	// Prawy dolny r�g
	square[2][0] = square[0][0] + sideLength + floatRand(distort, sideLength);
	square[2][1] = square[0][1] - sideLength + floatRand(distort, sideLength);

	// Lewy dolny r�g
	square[3][0] = square[0][0] + floatRand(distort, sideLength);
	square[3][1] = square[0][1] - sideLength + floatRand(distort, sideLength);

	glBegin(GL_POLYGON);

	for (int corner = 0; corner < 4; corner++)
	{
		if (colors)
			glColor3fv(colors[corner]);
		glVertex2fv(square[corner]);
	}

	glEnd();
}

void drawSierpinski(point2 leftTopCorner, float sideLength, float distort, int levels)
{
	if (levels)
	{
		float lesserLength = (sideLength / 3.0f);
		point2* squareCorners = new point2[9];

		for (int row = 0; row < 3; row++)
		{
			for (int column = 0; column < 3; column++)
			{
				squareCorners[(3 * row) + column][0] = leftTopCorner[0] + ((float)column * lesserLength);
				squareCorners[(3 * row) + column][1] = leftTopCorner[1] - ((float)row * lesserLength);
			}
		}

		for (int i = 0; i < 9; i++)
		{
			if (i != 4)
				drawSierpinski(squareCorners[i], lesserLength, distort, levels - 1);
		}
	}
	else
	{
		color colorNext[4];

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				colorNext[i][j] = (rand() / static_cast<float>(RAND_MAX));
		}

		point2 distortedCorner;
		distortedCorner[0] = leftTopCorner[0] + floatRand(distort, sideLength);
		distortedCorner[1] = leftTopCorner[1] + floatRand(distort, sideLength);

		drawSquare(distortedCorner, sideLength, distort, colorNext);
	}
}

/*************************************************************************************/
// Funkcaja okre�laj�ca, co ma by� rysowane
// (zawsze wywo�ywana, gdy trzeba przerysowa� scen�)
/*************************************************************************************/
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawSierpinski(leftTopCorner, squareSide, distortion, levels);

	glFlush();
}
/*************************************************************************************/

void keys(unsigned char key, int x, int y)
{
	if (key == 'w' && distortion < 1.0f)
	{
		distortion += resolution;
		cout << "Distortion level: " << distortion << endl;
		RenderScene();
	}
	else if (key == 's' && distortion > 0.0f)
	{
		distortion -= resolution;
		cout << "Distortion level: " << distortion << endl;
		RenderScene();
	}
	else if (key == 'q' && levels < 6)
	{
		levels++;
		cout << "Depth level: " << levels << endl;
		RenderScene();
	}
	else if (key == 'a' && levels > 0)
	{
		levels--;
		cout << "Depth level: " << levels << endl;
		RenderScene();
	}
	else if (key == 'r')
	{
		RenderScene();
	}
}

/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wn�trza okna - ustawiono na szary
	srand(time(nullptr));
}

/*************************************************************************************/
// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego
void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna
{

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna
	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okre�lenie uk�adu wsp�rz�dnych obserwatora

	glLoadIdentity();
	// Okre�lenie przestrzeni ograniczaj�cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okre�lenie okna obserwatora.
	// Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
	// Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych    

	glLoadIdentity();
}

/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB

	glutCreateWindow("Dywan Sierpinskiego");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna

	glutKeyboardFunc(keys);

	MyInit();
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst�pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/