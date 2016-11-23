#include "Shader.h"


#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 600

int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;

// Forward declarations
void render();
void keyboard(unsigned char key, int, int y);
void reshape(GLsizei newwidth, GLsizei newheight);

int main(int argc, char** argv)
{
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutCreateWindow("Program 1");
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	// Cg functions
	Shader::initialize();
	Shader::createVertexProgram();
	Shader::createFragmentProgram();

	BmpImage image("texture.bmp");
	if (image.isLoaded())
	{
		Shader::loadTexture(image);
		glutMainLoop();
	}
	
	system("pause");
	cgDestroyProgram(Shader::cgVertexProgram);
	cgDestroyProgram(Shader::cgFragmentProgram);
	cgDestroyContext(Shader::cgContext);
	return 0;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cgGLBindProgram(Shader::cgVertexProgram);
	cgGLEnableProfile(Shader::cgVertexProfile);
	cgGLBindProgram(Shader::cgFragmentProgram);
	cgGLEnableProfile(Shader::cgFragmentProfile);
	cgGLEnableTextureParameter(Shader::cgFragmentParam_decal);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(-1.0, 1.0);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(1.0, 1.0);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(1.0, -1.0);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(-1.0, -1.0);
	glEnd();


	cgGLDisableProfile(Shader::cgVertexProfile);
	cgGLDisableProfile(Shader::cgFragmentProfile);
	cgGLDisableTextureParameter(Shader::cgFragmentParam_decal);

	glutSwapBuffers();
}

void reshape(GLsizei newwidth, GLsizei newheight)
{
	// Set the viewport to cover the new window
	glViewport(0, 0, width = newwidth, height = newheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, 0.0, 100.0);
	glMatrixMode(GL_MODELVIEW);

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