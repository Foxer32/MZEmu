#include "GLScreen.h"

GLScreen::GLScreen(int width, int height, QWidget* parent)
	: QOpenGLWidget(parent), screenBuffer(width, height, QImage::Format_RGB888)
{

}

GLScreen::~GLScreen()
{
	glDeleteTextures(1, &textureId);
}

void GLScreen::initializeGL()
{
	initializeOpenGLFunctions();

	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenBuffer.width(), screenBuffer.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer.bits());

	glDisable(GL_TEXTURE_2D);
}

void GLScreen::resizeGL(int w, int h)
{
	float wi = screenBuffer.width();
	float hi = screenBuffer.height();
	float ws = w;
	float hs = h;

	if ((ws / hs) > (wi / hi))
	{
		w = (wi * hs) / hi;
		h = hs;
	}
	else
	{
		w = ws;
		h = (hi * ws) / wi;
	}

	verticles[0] = -1.0f * (w / ws); verticles[1] = 1.0f * (h / hs);
	verticles[2] = 1.0f * (w / ws); verticles[3] = 1.0f * (h / hs);
	verticles[4] = 1.0f * (w / ws); verticles[5] = -1.0f * (h / hs);
	verticles[6] = -1.0f * (w / ws); verticles[7] = -1.0f * (h / hs);
}

void GLScreen::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenBuffer.width(), screenBuffer.height(), GL_RGB, GL_UNSIGNED_BYTE, screenBuffer.bits());

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(verticles[0], verticles[1], 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(verticles[2], verticles[3], 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(verticles[4], verticles[7], 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(verticles[6], verticles[5], 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	
	//repaint();
	//update();
}
