#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(images);
    MainWindow w;
    w.show();
    return a.exec();
}
