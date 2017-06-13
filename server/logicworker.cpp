/************************************
名称：logicworker.cpp
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：逻辑线程的执行者，在GUI线程外执行
版权：完全自主完成
************************************/

#include "logicworker.h"
#include "mainwidget.h"

#include "processmanager.h"
#include "characterfac.h"

LogicWorker::LogicWorker(MainWidget *parent) : QObject(0), mainWidget(parent)
{

}

LogicWorker::~LogicWorker(){
}

void LogicWorker::setPort(quint16 port){
  _port = port;
}


//功能：开始线程执行，等待客户端连接
void LogicWorker::run(){
  clientVec = new std::vector<Werewolf::Client>;
  tcpServer = new QTcpServer(this);

  if(!tcpServer->listen(QHostAddress::Any, _port)){
    emit listenError();
    return;
  }
  connect(tcpServer, &QTcpServer::newConnection, this, &LogicWorker::addClient);
}

//功能：添加新连接的客户端
//
//说明：该函数将发信号给GUI线程，更新显示的客户端连接数
void LogicWorker::addClient(){
  while(tcpServer->hasPendingConnections()){
    QTcpSocket *newsock = tcpServer->nextPendingConnection();
    clientVec->push_back(Werewolf::Client(newsock));
    emit hasNewClient();
  }
}

//功能：逻辑线程的主函数
void LogicWorker::startGame(){
  Werewolf::ProcessManager manager(clientVec);  //将客户端信息告知Processmanager和Characterfac
  Werewolf::Characterfac fac(clientVec, &manager);

  if(fac.set()) //如果正常分配了角色，则开始游戏
    manager.run();

  emit gameOver();  //告知GUI，游戏已经结束

  //vector不是QObject，需要手动delete，但不能在析构函数中释放（析构函数在主线程中被调用，主线程中delete将出错）
  delete clientVec;
}
