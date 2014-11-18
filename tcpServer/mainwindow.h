#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAction>
#include <QActionEvent>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    enum Mode { NavigationMode, AddingMode, EditingMode };
    void updateInterface(Mode mode);

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void connection();
    void deleteLater();
    void read();

private slots:
    void serverStart();
    void serverStop();
    void inputProblem();
    void setTime(int);

private:
    Ui::MainWindow *ui;
    std::vector<QString> problem;
    QTcpServer *tcpServer;
    QTcpSocket *socket;
    quint16 nextBlockSize;
    QAction *optAct;
    void createActions();
    void createMenus();
    void createToolBars();
};



#endif // MAINWINDOW_H
