#include "stdafx.h"
/*************************************************************************************/
// Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
// FileName, alokuje pamiêæ i zwraca wskaŸnik (pBits) do bufora w którym
// umieszczone s¹ dane.
// Ponadto udostêpnia szerokoœæ (ImWidth), wysokoœæ (ImHeight) obrazu
// tekstury oraz dane opisuj¹ce format obrazu wed³ug specyfikacji OpenGL
// (ImComponents) i (ImFormat).
// Jest to bardzo uproszczona wersja funkcji wczytuj¹cej dane z pliku TGA.
// Dzia³a tylko dla obrazów wykorzystuj¹cych 8, 24, or 32 bitowy kolor.
// Nie obs³uguje plików w formacie TGA kodowanych z kompresj¹ RLE.
/*************************************************************************************/

GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
	/*************************************************************************************/
	// Struktura dla nag³ówka pliku  TGA
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
	// Wartoœci domyœlne zwracane w przypadku b³êdu
	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile != NULL)
	{
		/*************************************************************************************/
		// Przeczytanie nag³ówka pliku
		fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

		/*************************************************************************************/
		// Odczytanie szerokoœci, wysokoœci i g³êbi obrazu
		*ImWidth = tgaHeader.width;
		*ImHeight = tgaHeader.height;
		sDepth = tgaHeader.bitsperpixel / 8;

		/*************************************************************************************/
		// Sprawdzenie, czy g³êbia spe³nia za³o¿one warunki (8, 24, lub 32 bity)
		if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		{
			pbitsperpixel = NULL;
		}
		else
		{
			/*************************************************************************************/
			// Obliczenie rozmiaru bufora w pamiêci
			lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

			/*************************************************************************************/
			// Alokacja pamiêci dla danych obrazu
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
