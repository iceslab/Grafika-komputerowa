#define _USE_MATH_DEFINES
#include "Shader.h"
#include <cmath>

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;

static float hSeparation = 0.0;
static float vSeparation = 0.0;
static float twisting = 0.0;
static bool animate = false;

// Forward declarations
void render();
void animation();
void reshape(GLsizei newwidth, GLsizei newheight);
void keyboard(unsigned char key, int, int y);

int main(int argc, char** argv)
{
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutCreateWindow("Program 1");
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(animation);

	// Cg functions
	Shader::initialize();
	Shader::createVertexProgram();
	Shader::createFragmentProgram();

	BmpImage image("texture.bmp");
	if (image.isLoaded())
	{
		Shader::loadTexture(image);
		animate = true;
		glutMainLoop();
	}

	system("pause");
	return 0;
}

void render()
{
	static const unsigned maxRows = 50;
	static const unsigned maxColumns = 50;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cgSetParameter2f(Shader::cgVertexParam_leftSeparation, hSeparation, vSeparation);
	cgSetParameter2f(Shader::cgVertexParam_rightSeparation, -hSeparation, -vSeparation);
	cgSetParameter1f(Shader::cgVertexParam_twisting, twisting);

	cgGLBindProgram(Shader::cgVertexProgram);
	cgGLEnableProfile(Shader::cgVertexProfile);
	cgGLBindProgram(Shader::cgFragmentProgram);
	cgGLEnableProfile(Shader::cgFragmentProfile);
	cgGLEnableTextureParameter(Shader::cgFragmentParam_decal);

	for (unsigned row = 0; row < maxRows; row++)
	{
		glBegin(GL_QUAD_STRIP);
		for (unsigned column = 0; column <= maxColumns; column++)
		{
			float texX = static_cast<float>(column) / static_cast<float>(maxColumns);
			float texY = 1.0 - static_cast<float>(row + 1) / static_cast<float>(maxRows);
			float x = -1.0 + 2.0 * (static_cast<float>(column) / static_cast<float>(maxColumns));
			float y = 1.0 - 2.0 * (static_cast<float>(row + 1) / static_cast<float>(maxRows));
			
			glTexCoord2f(texX, texY);
			glVertex2f(x, y);

			texY = 1.0 - static_cast<float>(row) / static_cast<float>(maxRows);
			y = 1.0 - 2.0 * (static_cast<float>(row) / static_cast<float>(maxRows));
			
			glTexCoord2f(texX, texY);
			glVertex2f(x, y);
		}
		glEnd();
	}

	cgGLDisableProfile(Shader::cgVertexProfile);
	cgGLDisableProfile(Shader::cgFragmentProfile);
	cgGLDisableTextureParameter(Shader::cgFragmentParam_decal);

	glutSwapBuffers();
}

void reshape(GLsizei newwidth, GLsizei newheight)
{
	auto viewportSize = min(newwidth, newheight);
	float deltaX = 0.0;
	float deltaY = 0.0;

	if (newwidth > newheight)
		deltaX = (newwidth - newheight) / 2.0;
	else
		deltaY = (newheight - newwidth) / 2.0;

	glViewport(deltaX, deltaY, viewportSize, viewportSize);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, 0.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void animation()
{
	static const float deltaV = 0.005;
	static const float deltaH = 0.007;
	static float deltaTwist = 0.02;
	
	static float vVelocity = 0.005;
	static float hVelocity = 0.805;

	static const float fullAngle = 2 * M_PI;

	vVelocity = (vVelocity >= fullAngle ? 0.0 : vVelocity + deltaV);
	hVelocity = (hVelocity >= fullAngle ? 0.0 : hVelocity + deltaH);

	vSeparation = sin(vVelocity) / 2.0;
	hSeparation = sin(hVelocity) / 2.0;

	if (twisting >= M_PI)
		deltaTwist = -0.02;
	else if (twisting <= 0.0)
		deltaTwist = 0.02;
	
	twisting += deltaTwist;

	Sleep(10);

	glutPostRedisplay();
}

void keyboard(unsigned char key, int, int y)
{
	switch (key)
	{
		case 27:
			cgDestroyProgram(Shader::cgVertexProgram);
			cgDestroyProgram(Shader::cgFragmentProgram);
			cgDestroyContext(Shader::cgContext);
			exit(0);
			break;
		default:
			break;
	}
}
