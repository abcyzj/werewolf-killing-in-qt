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
  tcpServer = new QTcpServer;

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
      newsock->write(QString("Connected.").toUtf8());
      qDebug() << "client added";
      qDebug() << "thread info" << thread();
      newsock->write(QString("Hello").toUtf8());
      clientVec->push_back(Werewolf::Client(newsock));
      emit hasNewClient();
    }
}

void LogicWorker::startGame(){
  Werewolf::ProcessManager manager(clientVec);
  Werewolf::Characterfac fac(clientVec, &manager);
  clientVec->back().print("Excited");
  qDebug() << "Starting game.";
//  if(fac.set())
//    manager.run();
  fac.set();
  emit gameOver();
  thread()->quit();
}
