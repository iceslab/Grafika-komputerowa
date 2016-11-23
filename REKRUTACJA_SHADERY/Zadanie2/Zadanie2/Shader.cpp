#include "Shader.h"

CGcontext Shader::cgContext = nullptr;

CGprofile Shader::cgVertexProfile = CGprofile::CG_PROFILE_UNKNOWN;
CGprogram Shader::cgVertexProgram = nullptr;
const char* Shader::vertexProgramFileName = "vertexShader.cg";
const char* Shader::vertexProgramEntryName = "vertexEntry";
CGparameter Shader::cgVertexParam_leftSeparation = nullptr;
CGparameter Shader::cgVertexParam_rightSeparation = nullptr;
CGparameter Shader::cgVertexParam_twisting = nullptr;

CGprofile Shader::cgFragmentProfile = CGprofile::CG_PROFILE_UNKNOWN;
CGprogram Shader::cgFragmentProgram = nullptr;
const char* Shader::fragmentProgramFileName = "fragmentShader.cg";
const char* Shader::fragmentProgramEntryName = "fragmentEntry";
CGparameter Shader::cgFragmentParam_decal = nullptr;

GLuint Shader::textureNames[textureNamesNumber] = { 0 };

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::initialize()
{
	// Cg initialization
	cgContext = cgCreateContext();
	cgGLSetDebugMode(CG_FALSE);
	cgSetParameterSettingMode(cgContext, CG_DEFERRED_PARAMETER_SETTING);

	cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

	// Generate texture names
	glGenTextures(textureNamesNumber, textureNames);
}

void Shader::createVertexProgram()
{
	cgGLSetOptimalOptions(cgVertexProfile);
	cgVertexProgram =
		cgCreateProgramFromFile(cgContext,              /* Cg runtime context */
								CG_SOURCE,              /* Program in human-readable form */
								vertexProgramFileName,  /* Name of file containing program */
								cgVertexProfile,        /* Profile: OpenGL ARB vertex program */
								vertexProgramEntryName, /* Entry function name */
								NULL);                  /* No extra compiler options */
	cgVertexParam_leftSeparation =
		cgGetNamedParameter(cgVertexProgram, "leftSeparation");
	cgVertexParam_rightSeparation =
		cgGetNamedParameter(cgVertexProgram, "rightSeparation");
	cgVertexParam_twisting =
		cgGetNamedParameter(cgVertexProgram, "twisting");
	cgGLLoadProgram(cgVertexProgram);
}

void Shader::createFragmentProgram()
{
	cgGLSetOptimalOptions(cgFragmentProfile);
	cgFragmentProgram =
		cgCreateProgramFromFile(cgContext,              /* Cg runtime context */
								CG_SOURCE,              /* Program in human-readable form */
								fragmentProgramFileName,  /* Name of file containing program */
								cgFragmentProfile,        /* Profile: OpenGL ARB vertex program */
								fragmentProgramEntryName, /* Entry function name */
								NULL);                  /* No extra compiler options */
	cgGLLoadProgram(cgFragmentProgram);
}

void Shader::loadTexture(BmpImage &image)
{
	glBindTexture(GL_TEXTURE_2D, textureNames[0]);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  GL_RGB8,
					  image.getWidth(),
					  image.getHeight(),
					  GL_BGR_EXT,
					  GL_UNSIGNED_BYTE,
					  image.getData());
	glTexParameteri(GL_TEXTURE_2D,
					GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
}