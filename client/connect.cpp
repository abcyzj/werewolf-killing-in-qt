#include "mainwindow.h"
#include "connect.h"
#include "inputbox.h"

#include <QDockWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QSplitter>
#include <QDebug>
#include <QtNetwork>
#include <QListWidget>
#include <QtWidgets>

Connect::Connect(MainWindow *parent)
  : ipInputWidget(this), listDock(new QDockWidget(parent)), mainWin(parent),
    serverList(new QListWidget(listDock))
{
  QHBoxLayout *btnLayout = new QHBoxLayout(ipInputWidget);
  ipInputBox = new InputBox(tr("Input IP Address."), parent);
  ipInputBox->setFont(QFont("黑体", 20));
  btnLayout->addWidget(ipInputBox);
  QPushButton *connectBtn = new QPushButton(tr("Connect"), parent);
  btnLayout->addWidget(connectBtn);
  connect(ipInputBox, &QLineEdit::editingFinished, this, &Connect::connectViaIp);
  connect(connectBtn, &QPushButton::clicked, [=](){
      if(ipInputBox->hasAcceptableInput())
        connectViaIp();
    });
  ipInputWidget->setLayout(btnLayout);
  parent->splitter()->addWidget(ipInputWidget);
  ipInputWidget->hide();

  listDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  listDock->setAllowedAreas(Qt::RightDockWidgetArea);
  listDock->setWidget(serverList);
  mainWin->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, listDock);
  listDock->hide();
}

Connect::~Connect(){}

void Connect::begin(){
  qDebug() << "Connect begin";
  sock = new QUdpSocket(mainWin);
  sock->bind(mainWin->getPort(), QUdpSocket::ShareAddress);
  ipInputWidget->show();
  listDock->show();
  connect(sock, &QUdpSocket::readyRead, this, &Connect::processServInfo);
  connect(mainWin->getTcpSock(), &QTcpSocket::connected, mainWin, &MainWindow::CtoP);
  connect(serverList, &QListWidget::itemDoubleClicked, this, &Connect::connectToServ);
}

void Connect::clear(){
  disconnect(sock, &QUdpSocket::readyRead, this, &Connect::processServInfo);
  disconnect(mainWin->getTcpSock(), &QTcpSocket::connected, mainWin, &MainWindow::CtoP);
  disconnect(serverList, &QListWidget::itemDoubleClicked, this, &Connect::connectToServ);
  ipInputWidget->hide();
  listDock->hide();
  serverList->clear();
  sock->close();
}

void Connect::processServInfo(){
  while(sock->hasPendingDatagrams()){
      QByteArray datagram;
      datagram.resize(sock->pendingDatagramSize());
      sock->readDatagram(datagram.data(), datagram.size());

      bool ok = true;
      QString recvData(datagram);
      for(int i = 0; i < serverList->count(); i++){
          if(recvData ==
             serverList->item(i)->data(Qt::DisplayRole).toString()){
              ok = false;
              break;
            }
        }

      if(ok)
        serverList->addItem(recvData);
    }
}

void Connect::connectToServ(QListWidgetItem *serv){
  QString servInfo = serv->data(Qt::DisplayRole).toString();
  int pos = servInfo.indexOf(";");
  qDebug() << "Connect to serv.";
  if(pos >= 0){
      QString addr = servInfo.mid(pos + 4, servInfo.length() - pos - 5);
      mainWin->getTcpSock()->connectToHost(QHostAddress(addr), mainWin->getPort());
    }
}

void Connect::connectViaIp(){
  QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
  QRegExpValidator validator(rx, this);
  int pos = 0;
  QString IPAddr = ipInputBox->text();


  if(validator.validate(IPAddr, pos) == QRegExpValidator::Acceptable){
      QString addr = ipInputBox->text();
      mainWin->getTcpSock()->connectToHost(QHostAddress(addr), mainWin->getPort());
    }
  else{
      QMessageBox::information(this, tr("Incorrect IP address"), tr("You've entered a invalid IP address. Please check"));
    }
}
