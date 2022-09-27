#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	this->setFocusPolicy(Qt::StrongFocus);

	tapeBrowserWindow = new TapeBrowserWindow();

	configWindow();
	configSystem();

	setCentralWidget(screen);

	statusBar()->showMessage("Ready");
}

MainWindow::~MainWindow()
{
	GeneralThings::bus->stopSound();
	delete GeneralThings::bus;
	delete tapeBrowserWindow;
}

void MainWindow::configWindow()
{
	//resize(GeneralThings::bus->video.screenWidth, GeneralThings::bus->video.screenHeight);
	setWindowTitle("MZEmu");
	setWindowIcon(QIcon(":/icons/app.png"));

	mainToolBar = addToolBar("main toolbar");
	mainToolBar->setIconSize(QSize(32, 32));
	QMenu* fileMenu = menuBar()->addMenu("File");
	QMenu* viewMenu = menuBar()->addMenu("View");
	QMenu* machineMenu = menuBar()->addMenu("Machine");
	QMenu* toolsMenu = menuBar()->addMenu("Tools");
	QMenu* helpMenu = menuBar()->addMenu("Help");
	QMenu* customizeMenu;
	QMenu* sizeMenu;

//	============= File =============

	QAction* openAct = new QAction(QIcon(":/icons/open.png"), "Open", this);
	openAct->setShortcut(QKeySequence::Open);
	openAct->setStatusTip("Open supported file");
	connect(openAct, &QAction::triggered, this, &MainWindow::open);
	mainToolBar->addAction(openAct);
	fileMenu->addAction(openAct);
	this->addAction(openAct);

	QAction* saveAct = new QAction(QIcon(":/icons/save.png"), "Save", this);
	saveAct->setShortcut(QKeySequence::Save);
	saveAct->setStatusTip("Save snapshot");
	connect(saveAct, &QAction::triggered, this, &MainWindow::save);
	mainToolBar->addAction(saveAct);
	fileMenu->addAction(saveAct);
	this->addAction(saveAct);

	fileMenu->addSeparator();
	mainToolBar->addSeparator();

	QAction* exitAct = new QAction(QIcon(":/icons/exit.png"), "Exit", this);
	exitAct->setShortcut(QKeySequence::Quit);
	exitAct->setStatusTip("Quit this program");
	connect(exitAct, &QAction::triggered, this, &QApplication::quit);
	fileMenu->addAction(exitAct);
	this->addAction(exitAct);

	//	============= File =============
	//	============= Viev =============

	customizeMenu = viewMenu->addMenu("Customize");

	showToolBarAct = new QAction("Tool bar", this);
	showToolBarAct->setStatusTip("Show / hide tool bar");
	connect(showToolBarAct, &QAction::triggered, this, &MainWindow::toolBarShow);
	showToolBarAct->setCheckable(true);
	showToolBarAct->setChecked(true);
	customizeMenu->addAction(showToolBarAct);

	showStatusBarAct = new QAction("Status bar", this);
	showStatusBarAct->setStatusTip("Show / hide status bar");
	connect(showStatusBarAct, &QAction::triggered, this, &MainWindow::statusBarShow);
	showStatusBarAct->setCheckable(true);
	showStatusBarAct->setChecked(true);
	customizeMenu->addAction(showStatusBarAct);

	viewMenu->addSeparator();
	sizeMenu = viewMenu->addMenu("Size");

	QActionGroup* screenScaleActionGroup = new QActionGroup(this);
	screenScaleActionGroup->setExclusive(true);

	QAction* scale1x = new QAction("100%", this);
	scale1x->setStatusTip("Scale display 100%");
	connect(scale1x, &QAction::triggered, this, &MainWindow::screenScale1X);
	screenScaleActionGroup->addAction(scale1x);
	scale1x->setCheckable(true);
	sizeMenu->addAction(scale1x);

	QAction* scale2x = new QAction("200%", this);
	scale2x->setStatusTip("Scale display 200%");
	connect(scale2x, &QAction::triggered, this, &MainWindow::screenScale2X);
	screenScaleActionGroup->addAction(scale2x);
	scale2x->setCheckable(true);
	sizeMenu->addAction(scale2x);

	QAction* scale3x = new QAction("300%", this);
	scale3x->setStatusTip("Scale display 300%");
	connect(scale3x, &QAction::triggered, this, &MainWindow::screenScale3X);
	screenScaleActionGroup->addAction(scale3x);
	scale3x->setCheckable(true);
	scale3x->setChecked(true);
	sizeMenu->addAction(scale3x);

	QAction* scale4x = new QAction("400%", this);
	scale4x->setStatusTip("Scale display 400%");
	connect(scale4x, &QAction::triggered, this, &MainWindow::screenScale4X);
	screenScaleActionGroup->addAction(scale4x);
	scale4x->setCheckable(true);
	sizeMenu->addAction(scale4x);

	//	============= Viev =============
	//	============= Machine =============

	pauseAct = new QAction(QIcon(":/icons/pause.png"), "Pause", this);
	pauseAct->setCheckable(true);
	pauseAct->setShortcut(QKeySequence(Qt::Key_F1));
	pauseAct->setStatusTip("Pause and unpause the machine");
	connect(pauseAct, &QAction::triggered, this, &MainWindow::pause);
	mainToolBar->addAction(pauseAct);
	machineMenu->addAction(pauseAct);
	this->addAction(pauseAct);

	maxSpeedAct = new QAction(QIcon(":/icons/max_speed.png"), "Max speed", this);
	maxSpeedAct->setCheckable(true);
	maxSpeedAct->setShortcut(QKeySequence(Qt::Key_F2));
	maxSpeedAct->setStatusTip("Max speed");
	connect(maxSpeedAct, &QAction::triggered, this, &MainWindow::maxSpeed);
	mainToolBar->addAction(maxSpeedAct);
	machineMenu->addAction(maxSpeedAct);
	this->addAction(maxSpeedAct);

	machineMenu->addSeparator();

	QAction* softResetAct = new QAction(QIcon(":/icons/soft_reset.png"), "Soft reset", this);
	softResetAct->setShortcut(QKeySequence(Qt::Key_F3));
	softResetAct->setStatusTip("Reset the current machine");
	connect(softResetAct, &QAction::triggered, this, &MainWindow::softReset);
	mainToolBar->addAction(softResetAct);
	machineMenu->addAction(softResetAct);
	this->addAction(softResetAct);

	QAction* hardResetAct = new QAction(QIcon(":/icons/hard_reset.png"), "Hard reset", this);
	hardResetAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F3));
	hardResetAct->setStatusTip("Perform a hard machine reset");
	connect(hardResetAct, &QAction::triggered, this, &MainWindow::hardReset);
	machineMenu->addAction(hardResetAct);
	this->addAction(hardResetAct);

	QAction* nonMaskableInterruptAct = new QAction("NMI", this);
	nonMaskableInterruptAct->setStatusTip("Generate non maskable interrupt");
	connect(nonMaskableInterruptAct, &QAction::triggered, this, &MainWindow::nonMaskableInterrupt);
	machineMenu->addAction(nonMaskableInterruptAct);

	mainToolBar->addSeparator();

	QAction* fullScreenAct = new QAction(QIcon(":/icons/full_screen.png"), "Full screen", this);
	fullScreenAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Return));
	fullScreenAct->setStatusTip("Set full screen mode");
	connect(fullScreenAct, &QAction::triggered, this, &MainWindow::fullScreen);
	mainToolBar->addAction(fullScreenAct);
	viewMenu->addAction(fullScreenAct);
	this->addAction(fullScreenAct);

	//	============= Machine =============
	//	============= Tools =============

	QAction* tapeBrowserAct = new QAction(QIcon(":/icons/Speaker.png"), "Tape browser", this);
	tapeBrowserAct->setStatusTip("Open tape browser");
	connect(tapeBrowserAct, &QAction::triggered, this, &MainWindow::tapeBrowserShow);
	toolsMenu->addAction(tapeBrowserAct);

	//	============= Tools =============
	//	============= Help =============

	QAction* aboutAct = helpMenu->addAction("About MZEmu", this, &MainWindow::about);
	aboutAct->setStatusTip("Show info about application");

	QAction* aboutQtAct = helpMenu->addAction("About &Qt", this, &QApplication::aboutQt);
	aboutQtAct->setStatusTip("Show info about Qt library");

	//	============= Help =============
}

void MainWindow::configSystem()
{
	GeneralThings::bus = new Specrtum128kBus;

	screen = new Screen(GeneralThings::bus->video.screenWidth, GeneralThings::bus->video.screenHeight);

	QObject::connect(&GeneralThings::bus->wavPlayer, SIGNAL(updateProgressBar(int)), tapeBrowserWindow, SLOT(onUpdateProgressBar(int)));

	GeneralThings::bus->video.setScreen(screen);
	GeneralThings::bus->reset();
	GeneralThings::bus->setSampleFrequency(44100);
}

void MainWindow::open()
{
	std::string fileName = QFileDialog::getOpenFileName(this,"","","WAV files (*.wav)").toStdString();

	try
	{
		if (!fileName.empty())
		{
			auto stringEndsWith = [](const std::string& mainStr, const std::string& toMatch) {
				return (mainStr.size() >= toMatch.size() &&
					mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0);
			};

			if (stringEndsWith(fileName, ".wav"))
			{
				GeneralThings::bus->wavPlayer.readFile(fileName);
				tapeBrowserWindow->show();
			}

		}
	}
	catch (const std::exception &e)
	{
		QMessageBox::critical(this,"Error",e.what());
	}
}

void MainWindow::save()
{

}

void MainWindow::pause()
{
	GeneralThings::bus->setPausedStatus(pauseAct->isChecked());
}

void MainWindow::maxSpeed()
{
	GeneralThings::bus->setMaxSpeedStatus(maxSpeedAct->isChecked());
}

void MainWindow::softReset()
{
	GeneralThings::bus->reset();
}

void MainWindow::hardReset()
{
	GeneralThings::bus->reset(true);
}

void MainWindow::nonMaskableInterrupt()
{
	GeneralThings::bus->cpu.nonMaskableInterrupt();
}

void MainWindow::fullScreen()
{
	if (isFullScreen())
	{
		showBars();
		showNormal();
	}
	else
	{
		hideBars();
		showFullScreen();
	}
}

void MainWindow::screenScale1X()
{
	scaleWindow(0, 1);
}

void MainWindow::screenScale2X()
{
	scaleWindow(0, 2);
}

void MainWindow::screenScale3X()
{
	scaleWindow(0, 3);
}

void MainWindow::screenScale4X()
{
	scaleWindow(0, 4);
}

void MainWindow::toolBarShow()
{
	showToolBar = showToolBarAct->isChecked();
	updateBars();
}

void MainWindow::statusBarShow()
{
	showStatusBar = showStatusBarAct->isChecked();
	updateBars();
}

void MainWindow::tapeBrowserShow()
{
	tapeBrowserWindow->show();
}

void MainWindow::about()
{
	QMessageBox::about(this, "About MZEmu",
		"Emulator of ZX Spectrum 48k and 128k computers, and possibly other devices on the Z80 processor." 
		"<br>Github: <a href='https://github.com/Borys456/MZEmu'>https://github.com/Borys456/MZEmu</a>" );
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	GeneralThings::bus->keyboard.keyPressed(event->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
	GeneralThings::bus->keyboard.keyReleased(event->key());
}

void MainWindow::showEvent(QShowEvent* event)
{
	scaleWindow(2);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	tapeBrowserWindow->close();
}

void MainWindow::showBars()
{
	menuBar()->show();

	if (showToolBar)
		mainToolBar->show();

	if (showStatusBar)
		statusBar()->show();
}

void MainWindow::hideBars()
{
	menuBar()->hide();
	mainToolBar->hide();
	statusBar()->hide();
}

void MainWindow::updateBars()
{
	menuBar()->show();

	if (showToolBar)
		mainToolBar->show();
	else
		mainToolBar->hide();

	if (showStatusBar)
		statusBar()->show();
	else
		statusBar()->hide();

	scaleWindow();
}

void MainWindow::scaleWindow(int additionalHeight, int scale)
{
	if(scale > 0)
		scrrenScale = scale;

	int toolBarHeight = (mainToolBar->isHidden()) ? 0 : mainToolBar->height();
	int statusBarHeight = (statusBar()->isHidden()) ? 0 : statusBar()->height();

	resize(GeneralThings::bus->video.screenWidth * scrrenScale, GeneralThings::bus->video.screenHeight * scrrenScale + (menuBar()->height() + toolBarHeight + statusBarHeight) + additionalHeight);
}
