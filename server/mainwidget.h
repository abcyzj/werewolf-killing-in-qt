#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QUdpSocket>
#include <vector>

#include "logicthread.h"

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
  LogicThread gameLogicThread;

  QString getIP();

public slots:
  void gotNewClient();
  void gotListenError();

private slots:
  void begin();
  void broadcast(const QString &port, const QString &roomname);
  void startGame();
  void broadcastDatagram();
  void gameOver();
};

#endif // MAINWIDGET_H
