#pragma once
/*************************************************************************************/
//  Definicja materia³u z jakiego zrobiony jest czajnik
//  i definicja Ÿród³a œwiat³a
/*************************************************************************************/
/*************************************************************************************/
// Definicja materia³u z jakiego zrobiony jest czajnik
GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego               

GLfloat mat_shininess = { 20.0 };
// wspó³czynnik n opisuj¹cy po³ysk powierzchni

/*************************************************************************************/
// Definicja Ÿród³a œwiat³a
GLfloat light_position0[] = { 0.0, 0.0, 0.0, 1.0 };
// po³o¿enie Ÿród³a

GLfloat light_ambient0[] = { 1.0, 1.0, 1.0, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
// Ia = [Iar,Iag,Iab]

GLfloat light_diffuse0[] = { 1.0, 1.0, 0.0, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

GLfloat light_specular0[] = { 1.0, 1.0, 0.0, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
// odbicie kierunkowe Is = [Isr,Isg,Isb]


/*************************************************************************************/
// Definicja Ÿród³a œwiat³a
GLfloat light_position1[] = { 0.0, 0.0, 0.0, 1.0 };
// po³o¿enie Ÿród³a

GLfloat light_ambient1[] = { 0.5, 0.5, 0.5, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
// Ia = [Iar,Iag,Iab]

GLfloat light_diffuse1[] = { 1.0, 1.0, 0.0, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
// odbicie kierunkowe Is = [Isr,Isg,Isb]

GLfloat att_constant = { 1.0 };
// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji
// odleg³oœci od Ÿród³a

GLfloat att_linear = { 0.0005f };
// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji
// odleg³oœci od Ÿród³a

GLfloat att_quadratic = { 0.00001f };
// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
// odleg³oœci od Ÿród³a

void initLights()
{
	/*************************************************************************************/
	// Ustawienie parametrów materia³u i Ÿród³a œwiat³a
	/*************************************************************************************/
	// Ustawienie patrametrów materia³u
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametrów Ÿród³a
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	// Ustawienie parametrów Ÿród³a
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu oœwietlania sceny
	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	//glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora
	/*************************************************************************************/
}