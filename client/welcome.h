/*************************************************
名称：welcome.h
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：客户端欢迎界面类
版权：完全自行完成
*************************************************/

#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

class MainWindow;
class QWidget;
class QLineEdit;
class InputBox;

class Welcome : public QWidget{
  Q_OBJECT

 public:
  Welcome(MainWindow *parent);
  ~Welcome();
  void begin();
  void clear();

 private:
  MainWindow *mainWin;
  QWidget *welcomeWidget;
  InputBox *portInputBox;

  private slots:
    void gotPort();
};

#endif // WELCOME_H
