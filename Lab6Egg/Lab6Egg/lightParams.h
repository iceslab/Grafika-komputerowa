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
GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
// po�o�enie �r�d�a

GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
// Ia = [Iar,Iag,Iab]

GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
// odbicie kierunkowe Is = [Isr,Isg,Isb]

GLfloat att_constant = { 1.0 };
// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a

GLfloat att_linear = { 0.05f };
// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a

GLfloat att_quadratic = { 0.001f };
// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
// odleg�o�ci od �r�d�a
