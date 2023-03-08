#include "TapeBrowserWindow.h"
#include <filesystem>

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
	resize(400, 200);
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

	table = new QTableWidget(0, 3);
	table->setHorizontalHeaderLabels(QStringList() << "#" << "Block Details" << "Length");

	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setFocusPolicy(Qt::NoFocus);
	table->verticalHeader()->setVisible(false);
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	table->horizontalHeader()->setMinimumSectionSize(0);

	table->setColumnWidth(0, 20);
	table->setColumnWidth(2, 60);
	table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

	setCentralWidget(table);

	table->insertRow(table->rowCount());
	table->verticalHeader()->setSectionResizeMode(table->rowCount() - 1, QHeaderView::Stretch);
}

void TapeBrowserWindow::addRow(std::string data, int length)
{
	if (table->rowCount() == 0)
		table->insertRow(table->rowCount());

	table->insertRow(table->rowCount());

	table->verticalHeader()->setSectionResizeMode(table->rowCount() - 2, QHeaderView::Fixed);
	table->verticalHeader()->setSectionResizeMode(table->rowCount() - 1, QHeaderView::Stretch);

	QTableWidgetItem* col0 = new QTableWidgetItem(QString::number(table->rowCount() - 2));
	QTableWidgetItem* col1 = new QTableWidgetItem(QString::fromStdString(data));
	QTableWidgetItem* col2 = new QTableWidgetItem(QString::number(length));

	col0->setData(Qt::TextAlignmentRole, Qt::AlignCenter);

	table->setItem(table->rowCount() - 2, 0, col0);
	table->setItem(table->rowCount() - 2, 1, col1);
	table->setItem(table->rowCount() - 2, 2, col2);
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

	if (GeneralThings::bus->wavPlayer.getOpenedFileLength() != 0)
	{
		table->setRowCount(0);
		addRow(std::filesystem::path(GeneralThings::bus->wavPlayer.getOpenedFilePath()).filename().generic_string(), GeneralThings::bus->wavPlayer.getOpenedFileLength());
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

void TapeBrowserWindow::showEvent(QShowEvent* event)
{
	if (GeneralThings::bus->wavPlayer.getOpenedFileLength() != 0)
	{
		table->setRowCount(0);
		addRow(std::filesystem::path(GeneralThings::bus->wavPlayer.getOpenedFilePath()).filename().generic_string(), GeneralThings::bus->wavPlayer.getOpenedFileLength());
	}
}

void TapeBrowserWindow::closeEvent(QCloseEvent* event)
{
	GeneralThings::bus->wavPlayer.stop();
}
