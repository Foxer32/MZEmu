#include "Screen.h"

Screen::Screen(int width, int height, QWidget* parent)
	: QWidget(parent)
{
	screenBuffer = new QImage(width, height, QImage::Format_RGB888);
}

Screen::~Screen()
{
	delete screenBuffer;
}

void Screen::paintEvent(QPaintEvent* e)
{

	QPainter painter(this);
	painter.drawImage(0, 0, screenBuffer->scaled(width(), height()));
}


