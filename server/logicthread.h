#ifndef LOGICTHREAD_H
#define LOGICTHREAD_H

#include <QThread>
#include <QtNetwork/QTcpServer>
#include <vector>

#include"client.h"

class MainWidget;

class LogicThread : public QThread
{
  Q_OBJECT

 public:
  LogicThread(MainWidget *parent = 0);
  ~LogicThread();

  void setPort(quint16 port);

  public slots:
    void startGame();//一定选择队列连接

 protected:
    virtual void run();
};

#endif // LOGICTHREAD_H
