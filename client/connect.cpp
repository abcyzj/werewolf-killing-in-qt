/*************************************************
名称：connect.cpp
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：客户端主窗口类
版权：完全自行完成
*************************************************/

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

//功能：开始连接进程
void Connect::begin(){
  sock = new QUdpSocket(mainWin);
  sock->bind(mainWin->getPort(), QUdpSocket::ShareAddress);
  ipInputWidget->show();
  listDock->show();
  connect(sock, &QUdpSocket::readyRead, this, &Connect::processServInfo);
  connect(mainWin->getTcpSock(), &QTcpSocket::connected, mainWin, &MainWindow::CtoP);
  connect(serverList, &QListWidget::itemDoubleClicked, this, &Connect::connectToServ);
}


//功能：关闭连接线程
void Connect::clear(){
  disconnect(sock, &QUdpSocket::readyRead, this, &Connect::processServInfo);
  disconnect(mainWin->getTcpSock(), &QTcpSocket::connected, mainWin, &MainWindow::CtoP);
  disconnect(serverList, &QListWidget::itemDoubleClicked, this, &Connect::connectToServ);
  ipInputWidget->hide();
  listDock->hide();
  serverList->clear();
  sock->close();
}

//功能：解析收到的服务器广播的服务信息，并在GUI上更新
//说明：该函数是一个槽函数，与udpsocket的readyRead信号相连
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

//功能：连接服务器
//说明：该函数是一个槽函数，当用户点击服务器列表时被调用
void Connect::connectToServ(QListWidgetItem *serv){
  QString servInfo = serv->data(Qt::DisplayRole).toString();
  int pos = servInfo.indexOf(";");
  qDebug() << "Connect to serv.";
  if(pos >= 0){
    QString addr = servInfo.mid(pos + 4, servInfo.length() - pos - 5);
    mainWin->getTcpSock()->connectToHost(QHostAddress(addr), mainWin->getPort());
  }
}

//功能：通过IP地址与服务器连接，当用户输入不可用的IP时弹出消息框提示
//说明：该函数是一个槽函数，当用户完成IP地址输入时被调用。
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
