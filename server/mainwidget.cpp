#include "mainwidget.h"
#include "multiinputdialog.h"

#include "characterfac.h"
#include "processmanager.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QtNetwork/QTcpServer>

MainWidget::MainWidget(QWidget *parent)
  : QWidget(parent), beginBtn(new QPushButton(tr("Begin"), this)), showLabel(new QLabel(this)),
    startBtn(new QPushButton(tr("Start Game"), this)), broadcastSocket(this),
    gameLogicThread(this)
{
  //test
//  server.listen(QHostAddress::Any, 2896);

  worker.moveToThread(&gameLogicThread);
  showLabel->setText(tr("Click the Begin button to set up a server."));
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(showLabel);
  mainLayout->addWidget(beginBtn);
  mainLayout->addWidget(startBtn);
  startBtn->hide();
  setLayout(mainLayout);
  gameLogicThread.start();
  initSignalSlot();
}

MainWidget::~MainWidget()
{

}

void MainWidget::initSignalSlot(){
  connect(beginBtn, &QPushButton::clicked, this, &MainWidget::begin);
  connect(beginBtn, &QPushButton::clicked, &worker, &LogicWorker::run);
  connect(startBtn, &QPushButton::clicked, this, &MainWidget::startGame);
  connect(startBtn, &QPushButton::clicked, &worker, &LogicWorker::startGame);
  connect(&worker, &LogicWorker::hasNewClient, this, &MainWidget::gotNewClient);
  connect(&worker, &LogicWorker::listenError, this, &MainWidget::gotListenError);

  //test
  //connect(&server, &QTcpServer::newConnection, this, &MainWidget::addClient);
}

void MainWidget::begin(){
  MultiInputDialog inputDialog(2, this);
  QStringList list;
  list << "Port:" << "Roomname:";
  inputDialog.SetLabelTexts(list);
  inputDialog.SetLabelsWidth(80);
  inputDialog.SetLineEditRegExp(0, QRegExp("^[1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]{1}|6553[0-5]$"));
  if(inputDialog.exec() == QDialog::Accepted){
      QString port = inputDialog.GetOneText(0);
      worker.setPort(port.toInt());
      QString roomname = inputDialog.GetOneText(1);
      if(!port.isEmpty() && !roomname.isEmpty())
        broadcast(port, roomname);
    }
}

void MainWidget::broadcast(const QString &port, const QString &roomname){
  _port = port;
  _roomname = roomname;
  connect(&broadcastTimer, &QTimer::timeout, this, &MainWidget::broadcastDatagram);
  broadcastTimer.start(100);

  gameLogicThread.start();

  showLabel->setText("0 clients connected.");
  beginBtn->hide();
  startBtn->show();
}

QString MainWidget::getIP(){
  QList<QHostAddress> list = QNetworkInterface::allAddresses();
  foreach(QHostAddress addr, list){
      if(addr.protocol() == QAbstractSocket::IPv4Protocol && addr.toString() != "127.0.0.1")
        return addr.toString();
    }
  return "127.0.0.1";//找不到，只能返回无用地址
}

void MainWidget::broadcastDatagram(){
  QString msg = "ROOMNAME:" + _roomname + ";IP:" + getIP() + ";";
  broadcastSocket.writeDatagram(msg.toUtf8(), QHostAddress::Broadcast, _port.toInt());
}

void MainWidget::startGame(){
  qDebug() << "Main thread info: " << thread();
  broadcastTimer.stop();
  connect(&worker, &LogicWorker::gameOver, this, &MainWidget::gameOver);
  startBtn->hide();
  showLabel->setText(tr("Game started."));
}

void MainWidget::gameOver(){
  showLabel->setText(tr("Game over. We hope you and your friends had a good time."));
}

void MainWidget::gotNewClient(){
  static int cnt = 0;
  cnt++;
  showLabel->setText(tr((QString::number(cnt) + " client(s) connected.").toUtf8().data()));
}

void MainWidget::gotListenError(){
  QMessageBox::warning(this, tr("TCP listen error."), tr("Cannot listen, please check."));
}

//test
//void MainWidget::addClient(){
//  qDebug() << "New client";
//  QTcpSocket *newsock = server.nextPendingConnection();
//  newsock->write(QString("PRINT:Hello;").toUtf8());
//  showLabel->setText("client(s) connected.");
//}
