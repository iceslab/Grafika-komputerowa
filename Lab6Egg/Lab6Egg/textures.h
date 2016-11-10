#include "stdafx.h"
/*************************************************************************************/
// Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
// FileName, alokuje pami�� i zwraca wska�nik (pBits) do bufora w kt�rym
// umieszczone s� dane.
// Ponadto udost�pnia szeroko�� (ImWidth), wysoko�� (ImHeight) obrazu
// tekstury oraz dane opisuj�ce format obrazu wed�ug specyfikacji OpenGL
// (ImComponents) i (ImFormat).
// Jest to bardzo uproszczona wersja funkcji wczytuj�cej dane z pliku TGA.
// Dzia�a tylko dla obraz�w wykorzystuj�cych 8, 24, or 32 bitowy kolor.
// Nie obs�uguje plik�w w formacie TGA kodowanych z kompresj� RLE.
/*************************************************************************************/

GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
	/*************************************************************************************/
	// Struktura dla nag��wka pliku  TGA
#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;

#pragma pack(8)
	FILE *pFile = NULL;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;
	/*************************************************************************************/
	// Warto�ci domy�lne zwracane w przypadku b��du
	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile != NULL)
	{
		/*************************************************************************************/
		// Przeczytanie nag��wka pliku
		fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

		/*************************************************************************************/
		// Odczytanie szeroko�ci, wysoko�ci i g��bi obrazu
		*ImWidth = tgaHeader.width;
		*ImHeight = tgaHeader.height;
		sDepth = tgaHeader.bitsperpixel / 8;

		/*************************************************************************************/
		// Sprawdzenie, czy g��bia spe�nia za�o�one warunki (8, 24, lub 32 bity)
		if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		{
			pbitsperpixel = NULL;
		}
		else
		{
			/*************************************************************************************/
			// Obliczenie rozmiaru bufora w pami�ci
			lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

			/*************************************************************************************/
			// Alokacja pami�ci dla danych obrazu
			pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

			if (pbitsperpixel != NULL)
			{
				if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
				{
					free(pbitsperpixel);
					pbitsperpixel = NULL;
				}
				else
				{
					/*************************************************************************************/
					// Ustawienie formatu OpenGL
					switch (sDepth)
					{
					case 3:
						*ImFormat = GL_BGR_EXT;
						*ImComponents = GL_RGB8;
						break;
					case 4:
						*ImFormat = GL_BGRA_EXT;
						*ImComponents = GL_RGBA8;
						break;
					case 1:
						*ImFormat = GL_LUMINANCE;
						*ImComponents = GL_LUMINANCE8;
						break;
					};
				}
			}
		}
		fclose(pFile);
	}

	return pbitsperpixel;
}
/*************************************************************************************/
