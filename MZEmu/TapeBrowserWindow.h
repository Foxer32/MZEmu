#pragma once

#include <QtWidgets/QMainWindow>
#include <QToolBar>
#include <QProgressBar>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>

#include "GeneralThings.h"

class TapeBrowserWindow : public QMainWindow
{
	Q_OBJECT

public:
	TapeBrowserWindow(QWidget* parent = nullptr);
	~TapeBrowserWindow();

public slots:
	void onUpdateProgressBar(int value);

protected:
	void closeEvent(QCloseEvent* event);

private slots:
	void open();
	void play();
	void pause();
	void stop();


private:
	void configWindow();

	QToolBar* mainToolBar;
	QProgressBar* progressBar;

};
