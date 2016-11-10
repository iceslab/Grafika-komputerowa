/*************************************************************************************/
// Trochê bardziej skomplikowany program wykorzystuj¹cy funkcje biblioteki OpenGL
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

	// Lewy górny róg
	square[0][0] = leftTopCorner[0] + floatRand(distort, sideLength);	// x
	square[0][1] = leftTopCorner[1] + floatRand(distort, sideLength);	// y

	// Prawy górny róg
	square[1][0] = square[0][0] + sideLength + floatRand(distort, sideLength);
	square[1][1] = square[0][1] + floatRand(distort, sideLength);

	// Prawy dolny róg
	square[2][0] = square[0][0] + sideLength + floatRand(distort, sideLength);
	square[2][1] = square[0][1] - sideLength + floatRand(distort, sideLength);

	// Lewy dolny róg
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
// Funkcaja okreœlaj¹ca, co ma byæ rysowane
// (zawsze wywo³ywana, gdy trzeba przerysowaæ scenê)
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
// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wnêtrza okna - ustawiono na szary
	srand(time(nullptr));
}

/*************************************************************************************/
// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego
void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna
{

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna
	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna urz¹dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okreœlenie uk³adu wspó³rzêdnych obserwatora

	glLoadIdentity();
	// Okreœlenie przestrzeni ograniczaj¹cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okreœlenie okna obserwatora.
	// Pozwala to zachowaæ w³aœciwe proporcje rysowanego obiektu
	// Do okreœlenia okna obserwatora s³u¿y funkcja glOrtho(...)

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych    

	glLoadIdentity();
}

/*************************************************************************************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
void main(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyœwietlania
	// GLUT_RGBA - model kolorów RGB

	glutCreateWindow("Dywan Sierpinskiego");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna

	glutKeyboardFunc(keys);

	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/