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
#include <QTableWidget>
#include <QHeaderView>
#include <string>

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
	void showEvent(QShowEvent* event);
	void closeEvent(QCloseEvent* event);

private slots:
	void open();
	void play();
	void pause();
	void stop();


private:
	void configWindow();
	void addRow(std::string data,int length);
	std::string getBaseName(std::string const& path);

	QTableWidget* table;

	QToolBar* mainToolBar;
	QProgressBar* progressBar;

};
