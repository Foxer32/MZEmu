#pragma once

#include <QtWidgets/QMainWindow>
#include <QApplication>
#include <QKeyEvent>
#include <QToolBar>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

#include "olcNoiseMaker.h"

#include "Specrtum128kBus.h"

#include "Screen.h"

#include <cstdint>

class MZEmu : public QMainWindow
{
    Q_OBJECT

public:
    MZEmu(QWidget* parent = nullptr);
    ~MZEmu();

private:
    static Specrtum128kBus bus;
    olcNoiseMaker<short>* noiseMaker;

    void configWindow();
    void configAudio();

    static float makeNoise(int nChanel, float dTime);

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
};
