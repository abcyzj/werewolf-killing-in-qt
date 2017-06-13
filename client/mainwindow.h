/*************************************************
名称：mainwindow.h
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：客户端主窗口类
版权：完全自行完成
*************************************************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSplitter;
class QTextEdit;
class Welcome;
class Connect;
class Play;
class QTcpSocket;

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  QSplitter *splitter();
  quint16 getPort();
  void setPort(quint16 port);
  void setTcpSock(QTcpSocket *sock);
  QTcpSocket* getTcpSock();
  void printText(const QString &);

  public slots:
    void PtoW();
    void WtoC();
    void CtoP();

 private:
    QSplitter *_splitter;
    QTextEdit *showBox;
    Welcome *welcomeProcess;
    Connect *connectProcess;
    Play *playProcess;

    quint16 _port;
    QTcpSocket *tcpSock;
};

#endif // MAINWINDOW_H
