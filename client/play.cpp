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

void Play::begin(){
  inputBox->disableInput();
  timer.start(20);
  connect(&timer, &QTimer::timeout, this, &Play::addAndExecuteOrder);
  inputWidget->show();
}

void Play::clear(){
  timer.stop();
  disconnect(&timer, &QTimer::timeout, this, &Play::addAndExecuteOrder);
  inputWidget->hide();
  order.clear();
}

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

void Play::sendToServ(){
  if(!inputBox->text().isEmpty()){
      QByteArray msg = inputBox->text().toUtf8();
      mainWin->getTcpSock()->write(msg);
      inputBox->clear();
      emit msgSent();
    }
}
