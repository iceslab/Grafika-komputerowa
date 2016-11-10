#pragma once
/*************************************************************************************/
//  Definicja materia�u z jakiego zrobiony jest czajnik
//  i definicja �r�d�a �wiat�a
/*************************************************************************************/
/*************************************************************************************/
// Definicja materia�u z jakiego zrobiony jest czajnik
GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego               

GLfloat mat_shininess = { 20.0 };
// wsp�czynnik n opisuj�cy po�ysk powierzchni

/*************************************************************************************/
// Definicja �r�d�a �wiat�a
GLfloat light_position0[] = { 0.0, 0.0, 0.0, 1.0 };
// po�o�enie �r�d�a

GLfloat light_ambient0[] = { 1.0, 1.0, 1.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
// Ia = [Iar,Iag,Iab]

GLfloat light_diffuse0[] = { 1.0, 1.0, 0.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

GLfloat light_specular0[] = { 1.0, 1.0, 0.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
// odbicie kierunkowe Is = [Isr,Isg,Isb]


/*************************************************************************************/
// Definicja �r�d�a �wiat�a
GLfloat light_position1[] = { 0.0, 0.0, 0.0, 1.0 };
// po�o�enie �r�d�a

GLfloat light_ambient1[] = { 0.5, 0.5, 0.5, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
// Ia = [Iar,Iag,Iab]

GLfloat light_diffuse1[] = { 1.0, 1.0, 0.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
// odbicie kierunkowe Is = [Isr,Isg,Isb]

GLfloat att_constant = { 1.0 };
// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a

GLfloat att_linear = { 0.0005f };
// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a

GLfloat att_quadratic = { 0.00001f };
// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a

void initLights()
{
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
	//glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora
	/*************************************************************************************/
}