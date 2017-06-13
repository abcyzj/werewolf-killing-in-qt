/************************************
名称：logicworker.h
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：逻辑线程的执行者，在GUI线程外执行
版权：完全自主完成
************************************/

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
};

#endif // LOGICWORKER_H
