#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QUdpSocket>
#include <vector>

#include "client.h"
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

  void testGame();


private:
  QPushButton *beginBtn;
  QLabel *showLabel;
  QPushButton *startBtn;
  QString _port;
  QString _roomname;
  QTimer broadcastTimer;
  QUdpSocket broadcastSocket;
  QTcpServer tcpServer;
  std::vector<Werewolf::Client> clients;
  LogicThread gameLogicThread;

  QString getIP();

private slots:
  void begin();
  void broadcast(const QString &port, const QString &roomname);
  void startGame();
  void broadcastDatagram();
  void addClient();
  void gameOver();
};

#endif // MAINWIDGET_H
