#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QUdpSocket>
#include <vector>
#include <QThread>

#include "logicworker.h"

class QPushButton;
class QLabel;
class QLineEdit;

class MainWidget : public QWidget
{
  Q_OBJECT

public:
  MainWidget(QWidget *parent = 0);
  ~MainWidget();


private:
  QPushButton *beginBtn;
  QLabel *showLabel;
  QPushButton *startBtn;
  QString _port;
  QString _roomname;
  QTimer broadcastTimer;
  QUdpSocket broadcastSocket;
  QThread gameLogicThread;
  LogicWorker worker;

  QString getIP();
  void initSignalSlot();

public slots:
  void gotNewClient();
  void gotListenError();

private slots:
  void begin();
  void broadcast(const QString &port, const QString &roomname);
  void startGame();
  void broadcastDatagram();
  void gameOver();

signals:
  void setDone();

  //test
//  void addClient();
//private:
//  QTcpServer server;
};

#endif // MAINWIDGET_H
