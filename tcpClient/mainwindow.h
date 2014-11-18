#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void done();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString message;

public slots:
    void connectServer();
    void sendString();
    void readTcpData();
    void inputFile();
    void connectSuccess();
    void closeSuccess();
    void updateMessage();
    void error();
    void closeConnection();

private:
  QTcpSocket *client;
  Ui::MainWindow *ui;
  QString answer;
  quint16 nextBlockSize;
};

#endif // MAINWINDOW_H
