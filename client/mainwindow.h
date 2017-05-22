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
