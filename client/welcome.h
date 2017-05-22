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
