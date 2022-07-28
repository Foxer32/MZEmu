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
#include <QMessageBox>

#include "olcNoiseMaker.h"
#include "Specrtum128kBus.h"
#include "Screen.h"

class MZEmu : public QMainWindow
{
    Q_OBJECT

public:
    MZEmu(QWidget* parent = nullptr);
    ~MZEmu();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void showEvent(QShowEvent* event);

private slots:
    void open();
    void save();
    void pause();
    void maxSpeed();
    void softReset();
    void hardReset();
    void nonMaskableInterrupt();
    void fullScreen();
    void screenScale1X();
    void screenScale2X();
    void screenScale3X();
    void screenScale4X();
    void toolBarShow();
    void statusBarShow();
    void about();

private:
    void configWindow();
    void configSystem();

    void showBars();
    void hideBars();
    void updateBars();
    void scaleWindow(int scale = -1);
    static float makeNoise(int nChanel, float dTime);

    int screenWidth = 320;
    int screenHeight = 256;
    int scrrenScale = 3;

    bool showToolBar = true;
    bool showStatusBar = true;

    QToolBar* mainToolBar;
    QAction* pauseAct;
    QAction* maxSpeedAct;
    QAction* showToolBarAct;
    QAction* showStatusBarAct;

    static Specrtum128kBus bus;
    olcNoiseMaker<short>* noiseMaker;
};
