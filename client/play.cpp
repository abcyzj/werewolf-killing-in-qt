/*************************************************
名称：play.cpp
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：客户端游戏界面类
版权：完全自行完成
*************************************************/

#include "play.h"

#include <QtWidgets>
#include <QTimer>
#include <QTcpSocket>

#include "inputbox.h"

Play::Play(MainWindow *parent)
  : mainWin(parent), inputWidget(this){
  QHBoxLayout *inputLayout = new QHBoxLayout(this);
  inputBox = new InputBox(tr("Input area."), this);
  QPushButton *inputBtn = new QPushButton(tr("Input"), this);
  connect(inputBox, &QLineEdit::editingFinished, this, &Play::sendToServ);
  connect(inputBtn, &QPushButton::clicked, this, &Play::sendToServ);
  inputLayout->addWidget(inputBox);
  inputLayout->addWidget(inputBtn);
  inputWidget->setLayout(inputLayout);
  mainWin->splitter()->addWidget(inputWidget);
  inputWidget->hide();
}

Play::~Play(){}

//功能：开始游戏进程
void Play::begin(){
  inputBox->disableInput();
  timer.start(20);
  connect(&timer, &QTimer::timeout, this, &Play::addAndExecuteOrder);
  inputWidget->show();
}

//功能：结束游戏进程
void Play::clear(){
  timer.stop();
  disconnect(&timer, &QTimer::timeout, this, &Play::addAndExecuteOrder);
  inputWidget->hide();
  order.clear();
}

//功能：tcpsocket读取信息、解析、并执行
//说明：在Play的事件循环中，使用计时器timer不断调用该函数
void Play::addAndExecuteOrder(){
  QTcpSocket *sock = mainWin->getTcpSock();
  if(sock->bytesAvailable()){
      QByteArray datagram = sock->read(sock->bytesAvailable());
      order += datagram.data();
    }

  while(order.indexOf(";") >= 0){
      int pos = order.indexOf(";");
      execute(order.mid(0, pos));
      order.remove(0, pos + 1);
    }
}

//功能：执行命令
//说明：addAndExecuteOrder()通过调用本函数来执行命令
void Play::execute(const QString &ord){
  if(ord.mid(0, 6) == "PRINT:"){
      mainWin->printText(ord.mid(6));
    }

  else if(ord == "TURN_ON"){
      inputBox->enableInput();
      connect(this, &Play::msgSent, inputBox, &InputBox::disableInput);
    }

  else if(ord == "TURN_OFF"){
      inputBox->disableInput();
    }

  else if(ord == "HOLD_ON"){
      inputBox->enableInput();
      disconnect(this, &Play::msgSent, inputBox, &InputBox::disableInput);
    }

  else if(ord == "SHUT_DOWN"){
      mainWin->printText("Game Over.");
      timer.stop();
      mainWin->getTcpSock()->close();
    }
}

//功能：将玩家输入的消息发给服务器
//说明：该函数是一个槽函数，在消息输入框得到输入后被调用
void Play::sendToServ(){
  if(!inputBox->text().isEmpty()){
      QByteArray msg = inputBox->text().toUtf8();
      mainWin->getTcpSock()->write(msg);
      inputBox->clear();
      emit msgSent();
    }
}
