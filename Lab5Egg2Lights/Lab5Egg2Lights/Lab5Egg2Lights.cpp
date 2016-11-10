#include "stdafx.h"
#include "lightParams.h"

/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi 
//  uk�adu wsp�rzednych
/*************************************************************************************/

inline float calcXUNormal(float u, float v)
{
	return (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(3.1415926535L * v);
}

inline float calcYUNormal(float u, float v)
{
	return 640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u;
}

inline float calcZUNormal(float u, float v)
{
	return (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(3.1415926535L * v);
}

inline float calcXVNormal(float u, float v)
{
	return M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(M_PI * v);
}

inline float calcYVNormal(float u, float v)
{
	return 0;
}

inline float calcZVNormal(float u, float v)
{
	return -M_PI * (90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(M_PI * v);
}

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
	int X = 0, Y = 1, Z = 2;

	glBegin(model);
	for (int k = 0; k < 3; k++)
	{
		float* U = eggNormalU[i[k]][j[k]];
		float* V = eggNormalV[i[k]][j[k]];
		point3 normal =
		{ U[Y] * V[Z] - U[Z] * V[Y],
		U[Z] * V[X] - U[X] * V[Z],
		U[X] * V[Y] - U[Y] * V[X] };

		float length = sqrt(pow(normal[0], 2.0) + pow(normal[1], 2.0) + pow(normal[2], 2.0));
		normal[0] /= length;
		normal[1] /= length;
		normal[2] /= length;

		if (i[k] == 0)
		{
			normal[0] = 0.0;
			normal[1] = -1.0;
			normal[2] = 0.0;
		}
		else if (i[k] == (N >> 1))
		{
			normal[0] = 0.0;
			normal[1] = 1.0;
			normal[2] = 0.0;
		}
		else if (i[k] >(N >> 1))
		{
			normal[0] = -normal[0];
			normal[1] = -normal[1];
			normal[2] = -normal[2];
		}

		glNormal3fv(normal);
		if (model == GL_POINTS || model == GL_LINE_LOOP)
			glColor3f(1.0f, 1.0f, 1.0f);
		else
			glColor3ubv(colors[i[k]][j[k]]);
		glVertex3fv(eggPoints[i[k]][j[k]]);
	}

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

				eggNormalU[i][j][0] = calcXUNormal(u, v);
				eggNormalU[i][j][1] = calcYUNormal(u, v);
				eggNormalU[i][j][2] = calcZUNormal(u, v);

				eggNormalV[i][j][0] = calcXVNormal(u, v);
				eggNormalV[i][j][1] = calcYVNormal(u, v);
				eggNormalV[i][j][2] = calcZVNormal(u, v);

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
			theta[0] -= delta_x*pix2angleH;    // modyfikacja k�ta obrotu o kat proporcjonalny
			float diff = delta_y*pix2angleV;
			if (theta[1] - diff < 90.0 && theta[1] - diff > -90.0)
				theta[1] -= delta_y*pix2angleV;    // modyfikacja k�ta obrotu o kat proporcjonalny

			while (theta[0] > 360.0)
				theta[0] -= 360.0;
			while (theta[0] < 0.0)
				theta[0] += 360.0;
		}
		else if (status == 2)
		{
			phi[0] -= delta_x*pix2angleH;    // modyfikacja k�ta obrotu o kat proporcjonalny
			float diff = delta_y*pix2angleV;
			if (phi[1] - diff < 90.0 && phi[1] - diff > -90.0)
				phi[1] -= delta_y*pix2angleV;    // modyfikacja k�ta obrotu o kat proporcjonalny
			while (phi[0] > 360.0)
				phi[0] -= 360.0;
			while (phi[0] < 0.0)
				phi[0] += 360.0;
		}
		const float factor = M_PI / 180.0f;
		light_position0[0] = R * cos(theta[0] * factor) * cos(theta[1] * factor);
		light_position0[1] = R * sin(theta[1] * factor);
		light_position0[2] = R * sin(theta[0] * factor) * cos(theta[1] * factor);

		light_position1[0] = R * cos(phi[0] * factor) * cos(phi[1] * factor);
		light_position1[1] = R * sin(phi[1] * factor);
		light_position1[2] = R * sin(phi[0] * factor) * cos(phi[1] * factor);

		if (status < 0)
			status = 0;
	}
	// Czyszczenie macierzy bie??cej
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// Zdefiniowanie po�o�enia obserwatora
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glPopMatrix();
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
	
	/*************************************************************************************/
	// Ustawienie parametr�w materia�u i �r�d�a �wiat�a
	/*************************************************************************************/
	// Ustawienie patrametr�w materia�u
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametr�w �r�d�a
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie parametr�w �r�d�a
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu o�wietlania sceny
	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora
	/*************************************************************************************/
	status = -1;
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
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		// jako pozycji poprzedniej
		y_pos_old = y;
		//y_pos_old = y;
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