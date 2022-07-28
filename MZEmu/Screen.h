#pragma once

#include <QWidget>

class Screen : public QWidget
{
public:
    Screen(int width, int height, QWidget* parent = nullptr);

    QImage screenBuffer;

protected:
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent* event);

private:
    int w = 0;
    int h = 0;
    int x = 0;
    int y = 0;
};

