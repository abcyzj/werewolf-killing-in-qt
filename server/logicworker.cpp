#include "logicworker.h"
#include "mainwidget.h"

#include "processmanager.h"
#include "characterfac.h"

LogicWorker::LogicWorker(MainWidget *parent) : QObject(0), mainWidget(parent)
{

}

LogicWorker::~LogicWorker(){
  delete clientVec;
  delete tcpServer;
}

void LogicWorker::setPort(quint16 port){
  _port = port;
}

void LogicWorker::run(){
  clientVec = new std::vector<Werewolf::Client>;
  tcpServer = new QTcpServer(this);

  qDebug() << "Worker running.";

  qDebug() << thread();

  qDebug() << "Here";

  if(!tcpServer->listen(QHostAddress::Any, _port)){
      emit listenError();
      return;
    }
  connect(tcpServer, &QTcpServer::newConnection, this, &LogicWorker::addClient);
  qDebug() << "run over";
}

void LogicWorker::addClient(){
  while(tcpServer->hasPendingConnections()){
      QTcpSocket *newsock = tcpServer->nextPendingConnection();
      clientVec->push_back(Werewolf::Client(newsock));
      emit hasNewClient();
    }
}

void LogicWorker::startGame(){
  Werewolf::ProcessManager manager(clientVec);
  Werewolf::Characterfac fac(clientVec, &manager);
  if(fac.set())
    manager.run();
//  clientVec->at(0).print("Hello");
//  clientVec->at(0).hold_on_input();
//  for(int i = 0; i < 2; i++)
//    qDebug() << QString(clientVec->at(0).recv().c_str());
//  clientVec->at(0).turn_off_input();
  emit gameOver();
}
