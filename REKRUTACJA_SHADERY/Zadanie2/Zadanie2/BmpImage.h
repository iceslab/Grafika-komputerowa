#pragma once

#include <fstream>

using namespace std;

class BmpImage
{
public:
	BmpImage(const char* path = nullptr);
	~BmpImage();

	unsigned getWidth() const;
	unsigned getHeight() const;
	unsigned char* getData();

	void loadImage(const char* path);
	bool isLoaded();

private:
	void purge();

	static const unsigned int bmpHeaderSize = 54;

	unsigned char header[bmpHeaderSize]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;

	unsigned int imageSize;   // = width*height*3
	unsigned char* data;
};

