#include "mainwidget.h"
#include <QApplication>

#include <QList>
#include <QNetworkInterface>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWidget w;
  //w.testGame();
  w.show();


  return a.exec();
}
