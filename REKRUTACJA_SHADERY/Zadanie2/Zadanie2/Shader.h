#pragma once

#include <Windows.h>
#include <string>
#include <gl/GL.h>
#include <gl/glut.h>
#include <fstream>
#include <iostream>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "BmpImage.h"

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();

	/* METHODS */
	static void initialize();

	static void createVertexProgram();
	static void createFragmentProgram();

	// Loads texture from bitmap
	static void loadTexture(BmpImage &image);

	/* FIELDS */
	static CGcontext cgContext;

	// Vertex shader
	static CGprofile cgVertexProfile;
	static CGprogram cgVertexProgram;
	static const char* vertexProgramFileName;
	static const char* vertexProgramEntryName;
	static CGparameter cgVertexParam_leftSeparation;
	static CGparameter cgVertexParam_rightSeparation;
	static CGparameter cgVertexParam_twisting;

	// Fragment shader
	static CGprofile cgFragmentProfile;
	static CGprogram cgFragmentProgram;
	static const char* fragmentProgramFileName;
	static const char* fragmentProgramEntryName;
	static CGparameter cgFragmentParam_decal;

	// Texture aka image
	static constexpr GLsizei textureNamesNumber = 1;
	static GLuint textureNames[textureNamesNumber];

};

