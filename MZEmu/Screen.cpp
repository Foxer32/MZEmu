#include "Screen.h"

#include <QPainter>

Screen::Screen(int width, int height, QWidget* parent)
	: QWidget(parent), screenBuffer(width, height, QImage::Format_RGB888)
{
	setStyleSheet("background-color: black");
}

void Screen::resizeEvent(QResizeEvent* event)
{
	float wi = screenBuffer.width();
	float hi = screenBuffer.height();
	float ws = width();
	float hs = height();

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

	y = (hs - h) / 2;
	x = (ws - w) / 2;
}

void Screen::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(x, y, w, h, QPixmap::fromImage(screenBuffer));
}
