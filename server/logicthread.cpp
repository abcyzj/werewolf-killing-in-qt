#include "logicthread.h"
#include "mainwidget.h"

#include "client.h"
#include "processmanager.h"
#include "characterfac.h"

LogicThread::LogicThread(MainWidget *parent)
  : QThread(parent), mainWidget(parent)
{

}

LogicThread::~LogicThread(){
  delete clientVec;
  delete tcpServer;
}

void LogicThread::setPort(quint16 port){
  _port = port;
}

void LogicThread::run(){
  clientVec = new std::vector<Werewolf::Client>;
  tcpServer = new QTcpServer;

  connect(this, &LogicThread::hasNewClient, mainWidget, &MainWidget::gotNewClient);
  connect(this, &LogicThread::listenError, mainWidget, &MainWidget::gotListenError);

  if(!tcpServer->listen(QHostAddress::Any, _port)){
      emit listenError();
      return;
    }

  connect(tcpServer, &QTcpServer::newConnection, this, &LogicThread::addClient);

  exec();
}

void LogicThread::addClient(){
 while(tcpServer->hasPendingConnections()){
     QTcpSocket *newsock = tcpServer->nextPendingConnection();
     clientVec->push_back(Werewolf::Client(newsock));
     emit hasNewClient();
   }
}

void LogicThread::startGame(){
  Werewolf::ProcessManager manager(clientVec);
  Werewolf::Characterfac fac(clientVec, &manager);
//  if(fac.set())
//    manager.run();
  fac.set();
  quit();
}
