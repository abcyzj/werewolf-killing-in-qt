#ifndef LOGICWORKER_H
#define LOGICWORKER_H

#include <QObject>
#include <QtNetwork/QTcpServer>

class MainWidget;

#include "client.h"

class LogicWorker : public QObject
{
  Q_OBJECT
 public:
  explicit LogicWorker(MainWidget *parent = 0);
  ~LogicWorker();

  void setPort(quint16 port);

 signals:
  void hasNewClient();
  void listenError();
  void gameOver();

  public slots:
    void startGame();
    void run();

 private:
    MainWidget *mainWidget;
    quint16 _port;
    std::vector<Werewolf::Client> *clientVec;
    QTcpServer *tcpServer;

    private slots:
      void addClient();


      //test
      //public:
      //  QTcpSocket *test_sock;
};

#endif // LOGICWORKER_H
