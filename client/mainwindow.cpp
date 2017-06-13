/*************************************************
名称：mainwindow.cpp
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：客户端主窗口类
版权：完全自行完成
*************************************************/


#include "mainwindow.h"
#include "welcome.h"
#include "connect.h"
#include "play.h"

#include <QSplitter>
#include <QTextEdit>
#include <QTcpSocket>


//功能：初始化主窗口控件
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


//功能：将主窗口从Play切换到Welcome
void MainWindow::PtoW(){
  playProcess->clear();
  welcomeProcess->begin();
}

//功能：将主窗口从Welcome切换到Connect
void MainWindow::WtoC(){
  welcomeProcess->clear();
  connectProcess->begin();
}

//功能：将主窗口从Connect切换到Play
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

//功能：在主窗口展示区打印文本信息
//参数:
//    text:欲打印的信息
void MainWindow::printText(const QString &text){
  showBox->append(text);
}
