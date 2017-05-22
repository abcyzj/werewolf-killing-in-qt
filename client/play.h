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
