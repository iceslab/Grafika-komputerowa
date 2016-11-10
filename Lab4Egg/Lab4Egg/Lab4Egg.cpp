#include "stdafx.h"

/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi 
//  uk�adu wsp�rzednych
/*************************************************************************************/
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
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
// przerysowa� scen�)
void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej
	if (status)
	{
		if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
		{
			theta[0] += delta_x*pix2angleH;    // modyfikacja k�ta obrotu o kat proporcjonalny
			theta[1] += delta_y*pix2angleV;    // modyfikacja k�ta obrotu o kat proporcjonalny
		}
		else if (status == 2)
		{
			R += delta_y * pix2angleV;
		}
		const float factor = M_PI / 180.0f;
		viewer[0] = R * cos(theta[0] * factor) * cos(theta[1] * factor);
		viewer[1] = R * sin(theta[1] * factor);
		viewer[2] = R * sin(theta[0] * factor) * cos(theta[1] * factor);
	}
	// Czyszczenie macierzy bie??cej
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// Zdefiniowanie po�o�enia obserwatora

	glTranslated(0.0, -5.0, 0.0);

	Egg();

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			// Pierwszy poziom (sp�d po przedniej stronie jajka)
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
			// Ostatni poziom (sp�d po tylnej stronie jajka)
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
			// Normalne wype�nianie
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
	// Przekazanie polece� rysuj�cych do wykonania
	glutSwapBuffers();
}

/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
	srand(time(NULL));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny
}

/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angleH = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
	pix2angleV = 360.0 / (float)vertical;
	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji
	glLoadIdentity();
	// Czyszcznie macierzy bie��cej
	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

}

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		// jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y_pos_old = y;
		status = 2;
	}
	else
		status = 0;          // nie zosta� wci�ni�ty �aden klawisz
}

/*************************************************************************************/
// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie

	delta_y = y - y_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
	y_pos_old = y;            // podstawienie bie��cego po�o�enia jako poprzednie

	glutPostRedisplay();     // przerysowanie obrazu sceny
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

	glutPostRedisplay(); //od�wie�enie zawarto�ci aktualnego okna
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

			glutPostRedisplay(); //od�wie�enie zawarto�ci aktualnego okna
		}
	}
}

/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
void main(void)
{
	stringstream title;
	title << "Uk�ad wsp�rz�dnych 3-D, N = " << N;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(300, 300);
	glutCreateWindow(title.str().c_str());

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem
	// gdy zajdzie potrzba przeryswania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna                        

	glutIdleFunc(spinEgg);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}
/*************************************************************************************/