#include "mainwidget.h"
#include "multiinputdialog.h"

#include "client.h"
#include "characterfac.h"
#include "processmanager.h"

#include <QtWidgets>
#include <QtNetwork>

#include <QtTest/QTest>

MainWidget::MainWidget(QWidget *parent)
  : QWidget(parent), beginBtn(new QPushButton(tr("Begin"), this)), showLabel(new QLabel(this)),
    startBtn(new QPushButton(tr("Start Game"), this)), broadcastSocket(this), tcpServer(0),
    gameLogicThread(&clients)
{
  showLabel->setText(tr("Click the Begin button to set up a server."));
  connect(beginBtn, &QPushButton::clicked, this, &MainWidget::begin);
  connect(startBtn, &QPushButton::clicked, this, &MainWidget::startGame);
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(showLabel);
  mainLayout->addWidget(beginBtn);
  mainLayout->addWidget(startBtn);
  startBtn->hide();
  setLayout(mainLayout);
}

MainWidget::~MainWidget()
{

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

  if(!tcpServer.listen(QHostAddress::Any, _port.toInt())){
      QMessageBox::warning(this, tr("TCP listen error."), tr("Cannot listen, please check."));
      disconnect(&broadcastTimer, &QTimer::timeout, this, &MainWidget::broadcastDatagram);
      return;
    }

  showLabel->setText("0 clients connected.");
  connect(&tcpServer, &QTcpServer::newConnection, this, &MainWidget::addClient);
  beginBtn->hide();
  startBtn->show();
}

QString MainWidget::getIP(){
  QList<QHostAddress> list = QNetworkInterface::allAddresses();
  foreach(QHostAddress addr, list){
      if(addr.protocol() == QAbstractSocket::IPv4Protocol && addr.toString() != "127.0.0.1")
        return addr.toString();
    }
  return "127.0.0.1";//找不到，出错了
}

void MainWidget::broadcastDatagram(){
  QString msg = "ROOMNAME:" + _roomname + ";IP:" + getIP() + ";";
  broadcastSocket.writeDatagram(msg.toUtf8(), QHostAddress::Broadcast, _port.toInt());
}

void MainWidget::addClient(){
  QTcpSocket *newsock = tcpServer.nextPendingConnection();
  clients.emplace_back(newsock);
  clients.back().print("Hello");
  showLabel->setText(QString::number(clients.size()) + "client(s) connected.");
}

void MainWidget::startGame(){
  broadcastTimer.stop();
  for(auto &i: clients){
      i.moveToThread(&gameLogicThread);
    }
  connect(&gameLogicThread, &QThread::finished, this, &MainWidget::gameOver);
  gameLogicThread.start();
  startBtn->hide();
  showLabel->setText(tr("Game started."));
}

void MainWidget::gameOver(){
  showLabel->setText(tr("Game over. We hope you and your friends had a good time."));
}

void MainWidget::testGame(){
  startGame();
}
