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
GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
// po³o¿enie Ÿród³a

GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
// Ia = [Iar,Iag,Iab]

GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
// odbicie kierunkowe Is = [Isr,Isg,Isb]

GLfloat att_constant = { 1.0 };
// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji
// odleg³oœci od Ÿród³a

GLfloat att_linear = { 0.05f };
// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji
// odleg³oœci od Ÿród³a

GLfloat att_quadratic = { 0.001f };
// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
// odleg³oœci od Ÿród³a
