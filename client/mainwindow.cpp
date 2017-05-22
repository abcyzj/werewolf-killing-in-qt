#include "mainwindow.h"
#include "welcome.h"
#include "connect.h"
#include "play.h"

#include <QSplitter>
#include <QTextEdit>
#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), _splitter(new QSplitter(Qt::Vertical, this)),
    showBox(new QTextEdit(this)), tcpSock(new QTcpSocket)
{
  setMinimumSize(QSize(600, 600));
  showBox->setReadOnly(true);
  showBox->setFont(QFont("黑体", 20));
  showBox->setText(tr("Welcome to the world of Werewolf killing.\n"
                      "We hope you can enjoy your time.\n"
                      "Now input the port and begin."));
  _splitter->addWidget(showBox);
  welcomeProcess = new Welcome(this);
  connectProcess = new Connect(this);
  playProcess = new Play(this);
  setCentralWidget(_splitter);
  welcomeProcess->begin();
}

MainWindow::~MainWindow()
{

}

void MainWindow::PtoW(){
  playProcess->clear();
  welcomeProcess->begin();
}

void MainWindow::WtoC(){
  welcomeProcess->clear();
  connectProcess->begin();
}

void MainWindow::CtoP(){
  connectProcess->clear();
  playProcess->begin();
}

QSplitter* MainWindow::splitter(){
  return _splitter;
}

void MainWindow::setPort(quint16 port){
  _port = port;
}

quint16 MainWindow::getPort(){
  return _port;
}

void MainWindow::setTcpSock(QTcpSocket *sock){
  tcpSock = sock;
}

QTcpSocket* MainWindow::getTcpSock(){
  return tcpSock;
}

void MainWindow::printText(const QString &text){
  showBox->append(text);
}
