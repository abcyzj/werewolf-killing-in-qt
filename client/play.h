/*************************************************
名称：play.h
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：客户端游戏界面类
版权：完全自行完成
*************************************************/

#ifndef PLAY_H
#define PLAY_H

#include <QWidget>
#include <QTimer>

#include "mainwindow.h"

class InputBox;

class Play : public QWidget{
  Q_OBJECT

 public:
  Play(MainWindow* parent);
  ~Play();

  void begin();
  void clear();

 private:
  MainWindow *mainWin;
  QWidget *inputWidget;
  InputBox *inputBox;
  QTimer timer;
  QString order;

  void execute(const QString &);

  private slots:
    void addAndExecuteOrder();
    void sendToServ();

 signals:
    void msgSent();
};

#endif // PLAY_H
