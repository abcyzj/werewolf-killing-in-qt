/*******************************************
名称：main.cpp
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：主函数
版权：完全自行开发
*******************************************/

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
