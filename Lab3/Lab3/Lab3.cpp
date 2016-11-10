/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi 
//  uk³adu wspó³rzednych
/*************************************************************************************/
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
color3 colors[N][N];
bool calculated = false;
bool spin = true;
int model = GL_POINTS;
clock_t animationTime;
const float startingFps = 0.01f;
double frameDurationSec = startingFps;

inline float calcX(float u, float v)
{
	return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(3.1415926535L * v);
}

inline float calcY(float u, float v)
{
	return 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
}

inline float calcZ(float u, float v)
{
	return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(3.1415926535L * v);
}

void drawTriangle(array3 i, array3 j)
{

	glBegin(model);
	if (model == GL_POINTS || model == GL_LINE_LOOP)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor3ubv(colors[i[0]][j[0]]);
	glVertex3fv(eggPoints[i[0]][j[0]]);

	if (model == GL_POINTS || model == GL_LINE_LOOP)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor3ubv(colors[i[1]][j[1]]);
	glVertex3fv(eggPoints[i[1]][j[1]]);

	if (model == GL_POINTS || model == GL_LINE_LOOP)
		glColor3f(1.0f, 1.0f, 1.0f);
	else
		glColor3ubv(colors[i[2]][j[2]]);
	glVertex3fv(eggPoints[i[2]][j[2]]);
	glEnd();
}

void Egg(void)
{
	if (!calculated)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				float u = (float)i / (float)N, v = (float)j / (float)N;
				eggPoints[i][j][0] = calcX(u, v);
				eggPoints[i][j][1] = calcY(u, v);
				eggPoints[i][j][2] = calcZ(u, v);

				colors[i][j][0] = rand() & 0xff;
				colors[i][j][1] = rand() & 0xff;
				colors[i][j][2] = rand() & 0xff;
			}
		}
		calculated = true;
	}
}

/*************************************************************************************/
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba
// przerysowaæ scenê)
void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	glTranslated(0.0, -5.0, 0.0);

	Egg();

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			// Pierwszy poziom (spód po przedniej stronie jajka)
			if (i == 0)
			{
				array3 row = { 0, 1, 1 };
				array3 col = { j, j, j + 1 };

				if (j == (N - 1))
				{
					row[2] = N - 1;
					col[2] = 0;
				}
				drawTriangle(row, col);
			}
			// Ostatni poziom (spód po tylnej stronie jajka)
			else if (i == (N - 1))
			{
				array3 row = { i, 0, i };
				array3 col = { j, j, j + 1 };

				if (j == (N - 1))
				{
					row[2] = 1;
					col[2] = 0;
				}
				drawTriangle(row, col);
			}
			// Wszystkie brzegi
			else if (j == (N - 1))
			{
				array3 row = { i, i + 1, N - i };
				array3 col = { j, j, 0 };

				array3 row2 = { N - i, N - (i + 1), i + 1 };
				array3 col2 = { 0, 0, j };

				drawTriangle(row, col);
				drawTriangle(row2, col2);
			}
			// Normalne wype³nianie
			else
			{
				array3 row = { i, i + 1, i };
				array3 col = { j, j, j + 1 };

				array3 row2 = { i, i + 1, i + 1 };
				array3 col2 = { j + 1, j + 1, j };

				drawTriangle(row, col);
				drawTriangle(row2, col2);
			}
		}
	}

	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
	glutSwapBuffers();
}

/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania
void MyInit(void)
{
	srand(time(NULL));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny
}

/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
	// wymiarów okna 

	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej           

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okreœlenie tak zwanej
	// przestrzeni ograniczaj¹cej pozwalaj¹cej zachowaæ w³aœciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj¹cej s³u¿y funkcja
	// glOrtho(...)            

	if (horizontal <= vertical)
		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);

	else
		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu                                   

	glLoadIdentity();
	// Czyszcenie macierzy bie¿¹cej

}

void keyboard(unsigned char key, int x, int y)
{
	const float res = 2.0f;
	const float fpsChange = 0.001f;
	switch (key)
	{
	case 's':
		theta[0] -= res;
		if (theta[0] > 360.0) theta[0] -= 360.0;
		break;
	case 'w':
		theta[0] += res;
		if (theta[0] < 360.0) theta[0] += 360.0;
		break;
	case 'e':
		theta[1] -= res;
		if (theta[1] > 360.0) theta[1] -= 360.0;
		break;
	case 'q':
		theta[1] += res;
		if (theta[1] < 360.0) theta[1] += 360.0;
		break;
	case 'd':
		theta[2] -= res;
		if (theta[2] > 360.0) theta[2] -= 360.0;
		break;
	case 'a':
		theta[2] += res;
		if (theta[2] < 360.0) theta[2] += 360.0;
		break;
	case 'r':
		Egg();
		calculated = true;
		break;
	case '+':
		if (frameDurationSec > fpsChange)
		{
			frameDurationSec -= fpsChange;
			cout << "Frame duration: " << frameDurationSec << " s" << endl;
		}
		break;
	case '-':
		if (frameDurationSec < startingFps)
		{
			frameDurationSec += fpsChange;
			cout << "Frame duration: " << frameDurationSec << " s" << endl;
		}
		break;
	case ' ':
		spin = !spin;
		break;
	case '1':
		model = GL_POINTS;
		break;
	case '2':
		model = GL_LINE_LOOP;
		break;
	case '3':
		model = GL_TRIANGLES;
		break;
	default:
		break;
	}

	glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}

void spinEgg()
{
	double timeInSec = (clock() - animationTime) / double(CLOCKS_PER_SEC);

	if (spin)
	{
		if (timeInSec > frameDurationSec)
		{
			theta[0] -= 0.1;
			if (theta[0] > 360.0) theta[0] -= 360.0;

			theta[1] -= 0.1;
			if (theta[1] > 360.0) theta[1] -= 360.0;

			theta[2] -= 0.1;
			if (theta[2] > 360.0) theta[2] -= 360.0;

			animationTime = clock();

			glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
		}
	}
}

/*************************************************************************************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
void main(void)
{
	stringstream title;
	title << "Uk³ad wspó³rzêdnych 3-D, N = " << N;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow(title.str().c_str());

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem
	// gdy zajdzie potrzba przeryswania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// zazmiany rozmiaru okna                        

	glutIdleFunc(spinEgg);

	glutKeyboardFunc(keyboard);

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}
/*************************************************************************************/