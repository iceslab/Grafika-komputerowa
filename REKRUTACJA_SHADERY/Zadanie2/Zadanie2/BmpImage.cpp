#include "BmpImage.h"
#include <cstdio>

BmpImage::BmpImage(const char* path) :
	dataPos(0), width(0), height(0), imageSize(0), data(nullptr)
{
	loadImage(path);
}

BmpImage::~BmpImage()
{
	purge();
}

unsigned BmpImage::getWidth() const
{
	return width;
}

unsigned BmpImage::getHeight() const
{
	return height;
}

unsigned char* BmpImage::getData()
{
	return data;
}

void BmpImage::loadImage(const char* path)
{
	if (path != nullptr)
	{
		FILE * file = nullptr;
		fopen_s(&file, path, "rb");

		if (file != nullptr)
		{
			fill(header, header + bmpHeaderSize, '\0');
			purge();

			// When header is sucessfully read
			if (fread(header, 1, 54, file) == 54)
			{
				if (header[0] == 'B' && header[1] == 'M')
				{
					dataPos = *reinterpret_cast<int*>(&(header[0x0A]));
					imageSize = *reinterpret_cast<int*>(&(header[0x22]));
					width = *reinterpret_cast<int*>(&(header[0x12]));
					height = *reinterpret_cast<int*>(&(header[0x16]));

					if (imageSize == 0)
						// 3 : one byte for each Red, Green and Blue component
						imageSize = width * height * 3;
					if (dataPos == 0)
						dataPos = bmpHeaderSize; // The BMP header is done that way

					// Finally allocate buffer
					data = new unsigned char[imageSize];
					fread(data, 1, imageSize, file);
				}
			}

			fclose(file);
			file = nullptr;
		}
	}
}

bool BmpImage::isLoaded()
{
	return data != nullptr;
}

void BmpImage::purge()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
}