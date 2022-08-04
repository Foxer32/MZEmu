#include "MZEmu.h"

Specrtum128kBus MZEmu::bus = Specrtum128kBus();
WavPlayer MZEmu::wavPlayer = WavPlayer();

MZEmu::MZEmu(QWidget* parent)
	: QMainWindow(parent)
{
	Screen* screen = new Screen(screenWidth, screenHeight);
	setCentralWidget(screen);
	bus.video.setScreen(screen);

	configWindow();
	configSystem();

	statusBar()->showMessage("Ready");
}

MZEmu::~MZEmu()
{
	noiseMaker->Stop();
	//delete noiseMaker;
}

void MZEmu::configWindow()
{
	resize(screenWidth, screenHeight);
	setWindowTitle("MZEmu");
	setWindowIcon(QIcon(":/icons/app.png"));

	mainToolBar = addToolBar("main toolbar");
	mainToolBar->setIconSize(QSize(32, 32));
	QMenu* fileMenu = menuBar()->addMenu("&File");
	QMenu* viewMenu = menuBar()->addMenu("&View");
	QMenu* machineMenu = menuBar()->addMenu("&Machine");
	QMenu* toolsMenu = menuBar()->addMenu("&Tools");
	QMenu* helpMenu = menuBar()->addMenu("&Help");
	QMenu* customizeMenu;
	QMenu* sizeMenu;

//	============= File =============

	QAction* openAct = new QAction(QIcon(":/icons/open.png"), "&Open", this);
	openAct->setShortcut(QKeySequence::Open);
	openAct->setStatusTip("Open supported file");
	connect(openAct, &QAction::triggered, this, &MZEmu::open);
	mainToolBar->addAction(openAct);
	fileMenu->addAction(openAct);
	this->addAction(openAct);

	QAction* saveAct = new QAction(QIcon(":/icons/save.png"), "&Save", this);
	saveAct->setShortcut(QKeySequence::Save);
	saveAct->setStatusTip("Save snapshot");
	connect(saveAct, &QAction::triggered, this, &MZEmu::save);
	mainToolBar->addAction(saveAct);
	fileMenu->addAction(saveAct);
	this->addAction(saveAct);

	fileMenu->addSeparator();
	mainToolBar->addSeparator();

	QAction* exitAct = new QAction(QIcon(":/icons/exit.png"), "&Exit", this);
	exitAct->setShortcut(QKeySequence::Quit);
	exitAct->setStatusTip("Quit this program");
	connect(exitAct, &QAction::triggered, this, &QApplication::quit);
	fileMenu->addAction(exitAct);
	this->addAction(exitAct);

	//	============= File =============
	//	============= Viev =============

	customizeMenu = viewMenu->addMenu("&Customize");

	showToolBarAct = new QAction("&Tool bar", this);
	showToolBarAct->setStatusTip("Show / hide tool bar");
	connect(showToolBarAct, &QAction::triggered, this, &MZEmu::toolBarShow);
	showToolBarAct->setCheckable(true);
	showToolBarAct->setChecked(true);
	customizeMenu->addAction(showToolBarAct);

	showStatusBarAct = new QAction("&Status bar", this);
	showStatusBarAct->setStatusTip("Show / hide status bar");
	connect(showStatusBarAct, &QAction::triggered, this, &MZEmu::statusBarShow);
	showStatusBarAct->setCheckable(true);
	showStatusBarAct->setChecked(true);
	customizeMenu->addAction(showStatusBarAct);

	viewMenu->addSeparator();
	sizeMenu = viewMenu->addMenu("&Size");

	QActionGroup* screenScaleActionGroup = new QActionGroup(this);
	screenScaleActionGroup->setExclusive(true);

	QAction* scale1x = new QAction("&100%", this);
	scale1x->setStatusTip("Scale display 100%");
	connect(scale1x, &QAction::triggered, this, &MZEmu::screenScale1X);
	screenScaleActionGroup->addAction(scale1x);
	scale1x->setCheckable(true);
	sizeMenu->addAction(scale1x);

	QAction* scale2x = new QAction("&200%", this);
	scale2x->setStatusTip("Scale display 200%");
	connect(scale2x, &QAction::triggered, this, &MZEmu::screenScale2X);
	screenScaleActionGroup->addAction(scale2x);
	scale2x->setCheckable(true);
	sizeMenu->addAction(scale2x);

	QAction* scale3x = new QAction("&300%", this);
	scale3x->setStatusTip("Scale display 300%");
	connect(scale3x, &QAction::triggered, this, &MZEmu::screenScale3X);
	screenScaleActionGroup->addAction(scale3x);
	scale3x->setCheckable(true);
	scale3x->setChecked(true);
	sizeMenu->addAction(scale3x);

	QAction* scale4x = new QAction("&400%", this);
	scale4x->setStatusTip("Scale display 400%");
	connect(scale4x, &QAction::triggered, this, &MZEmu::screenScale4X);
	screenScaleActionGroup->addAction(scale4x);
	scale4x->setCheckable(true);
	sizeMenu->addAction(scale4x);

	//	============= Viev =============
	//	============= Machine =============

	pauseAct = new QAction(QIcon(":/icons/pause.png"), "&Pause", this);
	pauseAct->setCheckable(true);
	pauseAct->setShortcut(QKeySequence(Qt::Key_F1));
	pauseAct->setStatusTip("Pause and unpause the machine");
	connect(pauseAct, &QAction::triggered, this, &MZEmu::pause);
	mainToolBar->addAction(pauseAct);
	machineMenu->addAction(pauseAct);
	this->addAction(pauseAct);

	maxSpeedAct = new QAction(QIcon(":/icons/max_speed.png"), "&Max speed", this);
	maxSpeedAct->setCheckable(true);
	maxSpeedAct->setShortcut(QKeySequence(Qt::Key_F2));
	maxSpeedAct->setStatusTip("Max speed");
	connect(maxSpeedAct, &QAction::triggered, this, &MZEmu::maxSpeed);
	mainToolBar->addAction(maxSpeedAct);
	machineMenu->addAction(maxSpeedAct);
	this->addAction(maxSpeedAct);

	machineMenu->addSeparator();

	QAction* softResetAct = new QAction(QIcon(":/icons/soft_reset.png"), "&Soft reset", this);
	softResetAct->setShortcut(QKeySequence(Qt::Key_F3));
	softResetAct->setStatusTip("Reset the current machine");
	connect(softResetAct, &QAction::triggered, this, &MZEmu::softReset);
	mainToolBar->addAction(softResetAct);
	machineMenu->addAction(softResetAct);
	this->addAction(softResetAct);

	QAction* hardResetAct = new QAction(QIcon(":/icons/hard_reset.png"), "&Hard reset", this);
	hardResetAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F3));
	hardResetAct->setStatusTip("Perform a hard machine reset");
	connect(hardResetAct, &QAction::triggered, this, &MZEmu::hardReset);
	machineMenu->addAction(hardResetAct);
	this->addAction(hardResetAct);

	QAction* nonMaskableInterruptAct = new QAction("&NMI", this);
	nonMaskableInterruptAct->setStatusTip("Generate non maskable interrupt");
	connect(nonMaskableInterruptAct, &QAction::triggered, this, &MZEmu::nonMaskableInterrupt);
	machineMenu->addAction(nonMaskableInterruptAct);

	mainToolBar->addSeparator();

	QAction* fullScreenAct = new QAction(QIcon(":/icons/full_screen.png"), "&Full screen", this);
	fullScreenAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Return));
	fullScreenAct->setStatusTip("Set full screen mode");
	connect(fullScreenAct, &QAction::triggered, this, &MZEmu::fullScreen);
	mainToolBar->addAction(fullScreenAct);
	viewMenu->addAction(fullScreenAct);
	this->addAction(fullScreenAct);

	//	============= Machine =============
	//	============= Tools =============



	//	============= Tools =============
	//	============= Help =============

	QAction* aboutAct = helpMenu->addAction("&About MZEmu", this, &MZEmu::about);
	aboutAct->setStatusTip("Show info about application");

	QAction* aboutQtAct = helpMenu->addAction("About &Qt", this, &QApplication::aboutQt);
	aboutQtAct->setStatusTip("Show info about Qt library");

	//	============= Help =============
}

void MZEmu::configSystem()
{
	bus.reset();
	bus.cpu.cpuFrequency = 3500000;
	bus.setSampleFrequency(44100);

	wavPlayer.setSampleFrequency(44100);

	std::vector<std::wstring> devices = olcNoiseMaker<short>::Enumerate();

	noiseMaker = new olcNoiseMaker<short>(devices[0], 44100, 1, 8, 256);
	noiseMaker->SetUserFunction(makeNoise);
}

void MZEmu::open()
{
	std::string fileName = QFileDialog::getOpenFileName(this,"","","WAV files (*.wav)").toStdString();

	try
	{
		if (!fileName.empty())
		{
			wavPlayer.readFile(fileName);
			wavPlayer.play();
		}
	}
	catch (const std::exception &e)
	{
		QMessageBox::critical(this,"Error",e.what());
	}

}

void MZEmu::save()
{

}

void MZEmu::pause()
{
	bus.setPausedStatus(pauseAct->isChecked());
}

void MZEmu::maxSpeed()
{
	bus.setMaxSpeedStatus(maxSpeedAct->isChecked());
}

void MZEmu::softReset()
{
	bus.reset();
}

void MZEmu::hardReset()
{
	bus.reset(true);
}

void MZEmu::nonMaskableInterrupt()
{
	bus.cpu.nonMaskableInterrupt();
}

void MZEmu::fullScreen()
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

void MZEmu::screenScale1X()
{
	scaleWindow(1);
}

void MZEmu::screenScale2X()
{
	scaleWindow(2);
}

void MZEmu::screenScale3X()
{
	scaleWindow(3);
}

void MZEmu::screenScale4X()
{
	scaleWindow(4);
}

void MZEmu::toolBarShow()
{
	showToolBar = showToolBarAct->isChecked();
	updateBars();
}

void MZEmu::statusBarShow()
{
	showStatusBar = showStatusBarAct->isChecked();
	updateBars();
}

void MZEmu::about()
{
	QMessageBox::about(this, "About MZEmu",
		"Emulator of ZX Spectrum 48k and 128k computers, and possibly other devices on the Z80 processor." 
		"<br>Github: <a href='https://github.com/Borys456/MZEmu'>https://github.com/Borys456/MZEmu</a>" );
}

void MZEmu::keyPressEvent(QKeyEvent* event)
{
	bus.keyboard.keyPressed(event->key());
}

void MZEmu::keyReleaseEvent(QKeyEvent* event)
{
	bus.keyboard.keyReleased(event->key());
}

void MZEmu::showEvent(QShowEvent* event)
{
	scaleWindow();
}

void MZEmu::showBars()
{
	menuBar()->show();

	if (showToolBar)
		mainToolBar->show();

	if (showStatusBar)
		statusBar()->show();
}

void MZEmu::hideBars()
{
	menuBar()->hide();
	mainToolBar->hide();
	statusBar()->hide();
}

void MZEmu::updateBars()
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

void MZEmu::scaleWindow(int scale)
{
	if(scale > 0)
		scrrenScale = scale;

	int toolBarHeight = (mainToolBar->isHidden()) ? 0 : mainToolBar->height();
	int statusBarHeight = (statusBar()->isHidden()) ? 0 : statusBar()->height();

	resize(screenWidth * scrrenScale, screenHeight * scrrenScale + (menuBar()->height() + toolBarHeight + statusBarHeight));
}

float MZEmu::makeNoise(int nChanel, float dTime)
{
	bus.audioIn = wavPlayer.updateAudio();
	bus.clock();
	return bus.audioOut;
	return 0;
}
