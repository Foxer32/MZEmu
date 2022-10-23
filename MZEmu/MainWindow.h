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
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "GeneralThings.h"
#include "Specrtum128kBus.h"
#include "TapeBrowserWindow.h"
#include "GLScreen.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void showEvent(QShowEvent* event);
    void closeEvent(QCloseEvent* event);

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
    void tapeBrowserShow();
    void about();
    void keyboardHelp();

private:
    void configWindow();
    void configSystem();

    void showBars();
    void hideBars();
    void updateBars();
    void scaleWindow(int additionalHeight = 0 ,int scale = -1);

    int scrrenScale = 3;

    bool showToolBar = true;
    bool showStatusBar = true;

    GLScreen* screen;
    QToolBar* mainToolBar;
    QAction* pauseAct;
    QAction* maxSpeedAct;
    QAction* showToolBarAct;
    QAction* showStatusBarAct;

    TapeBrowserWindow* tapeBrowserWindow;
    QGraphicsView* keyboardHelpView;

};
