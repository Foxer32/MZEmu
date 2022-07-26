#pragma once

#include <QWidget>
#include <QPainter>

class Screen : public QWidget
{
public:
    Screen(int width, int height, QWidget* parent = nullptr);
    ~Screen();

    QImage* screenBuffer;

protected:
    void paintEvent(QPaintEvent*);
};

