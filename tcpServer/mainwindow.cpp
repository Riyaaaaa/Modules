#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createMenus();
    createToolBars();
    ui->setupUi(this);

    connect(ui->SStart,SIGNAL(clicked()),this,SLOT(serverStart()));
    connect(ui->SStop,SIGNAL(clicked()),this,SLOT(serverStop()));
    connect(ui->timeDial,SIGNAL(valueChanged(int)),this,SLOT(setTime(int)));
    connect(ui->setProb,SIGNAL(clicked()),this,SLOT(inputProblem()));

}

void MainWindow::serverStart()
{
    int port = 1024;
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, port);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(connection()));
    ui->SLOG->append("servet start...");
}

void MainWindow::serverStop()
{
    ui->SLOG->append("servet stop");
    tcpServer->close();
    delete tcpServer;
}

void MainWindow::createActions()
{
    optAct = new QAction(tr("&Option"), this);
    optAct->setStatusTip(tr("option settings"));
    connect(optAct, SIGNAL(triggered()), this, SLOT(inputProblem()));
}

void MainWindow::createMenus()
 {
    qDebug("menu");
    QMenu *fileMenu = menuBar()->addMenu(tr("&Option"));
    fileMenu->addAction(optAct);
 }

void MainWindow::createToolBars()
{
    QToolBar *toolbar = addToolBar(tr("Option"));
    toolbar->addAction(optAct);
}


void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


//set problem
void MainWindow::inputProblem()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("input Problem"), "",
                                                    tr("anct problem (*.txt);;All Files (*)"));
    if (fileName.isEmpty())return;
    else {
        QFile file(fileName);
        QString str;

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        //z
        QTextStream in(&file);
        while (!in.atEnd()) {
            str = in.readLine();
            problem.push_back(str);
        }
        for(int i=0;i<problem.size();i++)
            ui->SLOG->append(problem[i]);
    }

    //updateInterface(NavigationMode);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// connected client
void MainWindow::connection()
{
    ui->SLOG->append("new connection...");

    // get client socket
    socket = tcpServer->nextPendingConnection();

    //disconnect slot
    connect(socket, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    //Reception slot
    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));

    //get client address
    QString ippaddr = socket->localAddress().toString();
    int ippaddr2 = socket->localPort();

    ui->SLOG->append(QString("client ip =")+ippaddr);    //ip アドレス表示
    ui->SLOG->append(QString("port number =") + QString(ippaddr2));

    //クライアントに文字を送信するためのストリームを定義
    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_4_1);

    // データを送信。
    QString string = "riyasan_dayo";
    out << quint16(string.size()) <<  string << quint16(0xFFFF);

}

//disconnected client
void MainWindow::deleteLater()
{
    qDebug("%s", "deleteLater");
}

//Reception String
void MainWindow::read()
{
    ui->SLOG->append("on_readyRead");

    char buffer[128];
    QString string;

    socket->read(buffer, socket->bytesAvailable());
    std::string sString(buffer);
    QString qString(sString.c_str());

    ui->SLOG->append(qString);
    ui->SLOG->append("on_readyRead　end");

    QDataStream out(socket);
    out.setVersion(QDataStream::Qt_4_1);

     if(qString=="unko"){
        string = "ok";
     }
     else{
         string = "shine";
     }
    out << quint16(string.size()) <<  string << quint16(0xFFFF);
}

void MainWindow::setTime(int time){
    QString timeStr;
    QString sec;
    time*=10;
    timeStr.setNum(time/60);
    timeStr.append(':');
    sec.setNum(time%60);
    timeStr.append(sec);
    ui->timeLabel->setText(timeStr);
}

