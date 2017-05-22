#ifndef CONNECT_H
#define CONNECT_H

#include <QObject>

#include "mainwindow.h"

class QDockWidget;
class QUdpSocket;
class QListWidget;
class QListWidgetItem;
class InputBox;

class Connect: public QWidget{
  Q_OBJECT

public:
  Connect(MainWindow *parent);
  ~Connect();

  void begin();
  void clear();
  void setPort(quint16 port);

private:
  QWidget *ipInputWidget;
  QDockWidget *listDock;
  MainWindow *mainWin;
  QUdpSocket *sock;
  QListWidget *serverList;
  InputBox *ipInputBox;

private slots:
  void processServInfo();
  void connectToServ(QListWidgetItem *serv);
  void connectViaIp();
};

#endif // CONNECT_H
