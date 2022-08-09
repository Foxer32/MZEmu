#include "TapeBrowserWindow.h"

TapeBrowserWindow::TapeBrowserWindow(QWidget* parent)
	: QMainWindow(parent)
{
	configWindow();
}

TapeBrowserWindow::~TapeBrowserWindow()
{

}

void TapeBrowserWindow::configWindow()
{
	//resize(screenWidth, screenHeight);
	setWindowTitle("Tape browser");
	setWindowIcon(QIcon(":/icons/Speaker.png"));

	mainToolBar = addToolBar("main toolbar");
	mainToolBar->setIconSize(QSize(32, 32));

	QAction* openAct = new QAction(QIcon(":/icons/open.png"), "Open", this);
	openAct->setShortcut(QKeySequence::Open);
	openAct->setStatusTip("Open supported file");
	connect(openAct, &QAction::triggered, this, &TapeBrowserWindow::open);
	mainToolBar->addAction(openAct);
	this->addAction(openAct);

	mainToolBar->addSeparator();

	QAction* playAct = new QAction(QIcon(":/icons/play.png"), "Play", this);
	playAct->setStatusTip("Play sound");
	connect(playAct, &QAction::triggered, this, &TapeBrowserWindow::play);
	mainToolBar->addAction(playAct);

	QAction* pauseAct = new QAction(QIcon(":/icons/pause.png"), "Pause", this);
	pauseAct->setStatusTip("Pause sound");
	connect(pauseAct, &QAction::triggered, this, &TapeBrowserWindow::pause);
	mainToolBar->addAction(pauseAct);

	QAction* stopAct = new QAction(QIcon(":/icons/stop.png"), "Pause", this);
	stopAct->setStatusTip("Stop sound");
	connect(stopAct, &QAction::triggered, this, &TapeBrowserWindow::stop);
	mainToolBar->addAction(stopAct);

	mainToolBar->addSeparator();

	progressBar = new QProgressBar();
	progressBar->setMinimum(0);
	progressBar->setMaximum(100);
	progressBar->setAlignment(Qt::AlignCenter);
	progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mainToolBar->addWidget(progressBar);

}

void TapeBrowserWindow::open()
{
	
	std::string fileName = QFileDialog::getOpenFileName(this, "", "", "WAV files (*.wav)").toStdString();

	try
	{
		if (!fileName.empty())
		{
			auto stringEndsWith = [](const std::string& mainStr, const std::string& toMatch) {
				return (mainStr.size() >= toMatch.size() &&
					mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0);
			};

			if (stringEndsWith(fileName, ".wav"))
				GeneralThings::bus->wavPlayer.readFile(fileName);
		}
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(this, "Error", e.what());
	}
}

void TapeBrowserWindow::play()
{
	GeneralThings::bus->wavPlayer.play();
}

void TapeBrowserWindow::pause()
{
	GeneralThings::bus->wavPlayer.pause();
}

void TapeBrowserWindow::stop()
{
	GeneralThings::bus->wavPlayer.stop();
}

void TapeBrowserWindow::onUpdateProgressBar(int percents)
{
	progressBar->setValue(percents);
}

void TapeBrowserWindow::closeEvent(QCloseEvent* event)
{
	GeneralThings::bus->wavPlayer.stop();
}
