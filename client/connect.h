/*************************************************
名称：connect.h
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：客户端主窗口类
版权：完全自行完成
*************************************************/

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
