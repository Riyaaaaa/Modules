#include <QtGui>
#include <QtCore>
#include <QLabel>
#include <QApplication>
#include "mainwindow.h"
int main(int argc,char *argv[])
{
QApplication app(argc, argv);
MainWindow *x;
x = new MainWindow;
//app.setActiveWindow(x);
x->show();
return app.exec();
}//end of main program

