#include "MZEmu.h"

Specrtum128kBus MZEmu::bus = Specrtum128kBus();

MZEmu::MZEmu(QWidget* parent)
	: QMainWindow(parent)
{
	bus.reset();
	bus.cpu.cpuFrequency = 3500000;
	bus.setSampleFrequency(44100);

	Screen* screen = new Screen(320, 256);
	setCentralWidget(screen);
	bus.video.setScreen(screen);

	configWindow();
	configAudio();
}

MZEmu::~MZEmu()
{
	noiseMaker->Stop();
	//delete noiseMaker;
}

void MZEmu::configWindow()
{
	setFixedSize(960, 786 + 75);
	setWindowTitle("MZEmu");
	setWindowIcon(QIcon(":/icons/app.png"));

	QToolBar* toolbar = addToolBar("main toolbar");
	QAction* open = toolbar->addAction(QIcon(":/icons/open.png"), "Open");
	QAction* save = toolbar->addAction(QIcon(":/icons/save.png"), "Save");
	toolbar->addSeparator();
	QAction* pauseResume = toolbar->addAction(QIcon(":/icons/pause.png"), "Pause");
	QAction* reset = toolbar->addAction(QIcon(":/icons/reset.png"), "Reset");
	QAction* exit = toolbar->addAction(QIcon(":/icons/exit.png"), "Exit");
	connect(exit, &QAction::triggered, qApp, &QApplication::quit);

	QMenu* file;
	file = menuBar()->addMenu("File");
	file->addAction(open);
	file->addAction(save);
	file->addSeparator();
	file->addAction(exit);

	QMenu* viev;
	viev = menuBar()->addMenu("View");
	viev->addAction("Size");

	statusBar()->showMessage("Test");
}

void MZEmu::configAudio()
{
	std::vector<std::wstring> devices = olcNoiseMaker<short>::Enumerate();

	noiseMaker = new olcNoiseMaker<short>(devices[0], 44100, 1, 8, 256);
	noiseMaker->SetUserFunction(makeNoise);
}

void MZEmu::keyPressEvent(QKeyEvent* event)
{
	bus.keyboard.keyPressed(event->key());
}

void MZEmu::keyReleaseEvent(QKeyEvent* event)
{
	bus.keyboard.keyReleased(event->key());
}

float MZEmu::makeNoise(int nChanel, float dTime)
{
	//bus.audioIn = fSample;
	bus.clock();
	return bus.audioOut;
	return 0;
}
