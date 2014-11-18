#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->connectButton,SIGNAL(clicked()),this,SLOT(connectServer()));
    connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(sendString()));
    connect(ui->inputButton,SIGNAL(clicked()),this,SLOT(inputFile()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(closeConnection()));
}


MainWindow::~MainWindow()
{
    if(client->state()==3)client->close();
    if(client!=NULL)delete client;
    delete ui;
}


void MainWindow::connectServer()
{
    qDebug("conecting...");
    client = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;

    connect( client, SIGNAL(readyRead()), SLOT(readTcpData()) );
    connect(client, SIGNAL(connected()), this, SLOT(connectSuccess()));
    connect(client, SIGNAL(disconnected()),
                this, SLOT(closeSuccess()));
    connect(client, SIGNAL(readyRead()),
                this, SLOT(updateMessage()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(error()));

    client->connectToHost("localhost", 1024);
    if(!client->waitForConnected(100)){
        ui->log->appendPlainText("conection faild");
    }
}

void MainWindow::connectSuccess()
{
    ui->log->appendPlainText("connection succeeded");
}

void MainWindow::sendString(){
    QByteArray data;
    if(answer.size()==0){
        ui->log->appendPlainText("answer empty¥n");
        return;
    }
    if(client->state()!=3){
        ui->log->appendPlainText("connection state not established");
        return;
    }
    //QDataStream out(&data, QIODevice::WriteOnly);
    //out << quint16(string.size()) <<  string;
    data = answer.toUtf8();
     qDebug("sending... %s",qPrintable(QString(data)));
    if( client->waitForConnected() ) {
        client->write( data );
    }
}

void MainWindow::readTcpData()
{
    QByteArray data = client->readAll();
    QString str(data);
    qDebug("%s",qPrintable(str));
}

void MainWindow::inputFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("input Answer"), "",
                                                    tr("text files (*.txt);;All Files (*)"));
    if (fileName.isEmpty())return;
    else {
        QFile file(fileName);
        QString str;
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))return;
        QTextStream in(&file);
        while (!in.atEnd()) {
            str = in.readLine();
            answer.append(str);
        }
        ui->log->appendPlainText(answer);
    }

    //updateInterface(NavigationMode);
}

void MainWindow::updateMessage()
{
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_4_1);

    forever {
        if (nextBlockSize == 0) {
            if (client->bytesAvailable() < sizeof(quint16))
                break;
            in >> nextBlockSize;
        }
        if (nextBlockSize == 0xFFFF) {
            client->close();
            break;
        }
        if (client->bytesAvailable() < nextBlockSize)
            break;

        in >> message;

        nextBlockSize = 0;
    }
}

void MainWindow::closeConnection()
{
    client->close();
    emit done();
}

void MainWindow::closeSuccess()
{
    ui->log->appendPlainText("connection closed.");
}

void MainWindow::error()
{
    message = client->errorString();
    client->close();
    emit done();
}
