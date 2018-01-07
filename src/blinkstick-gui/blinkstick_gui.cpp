#include "blinkstick_gui.h"

#include <QApplication>

int main(int argc, char **argv)
{
 QApplication app (argc, argv);
 QMainWindow* lMainWindow = new QMainWindow();
 return app.exec();
}