/*******************************************
名称：client.cpp
作者：叶梓杰 计65 2016011380
时间：2017/05/22
内容：向逻辑模块提供的与客户端通信的接口类
版权：完全自行开发
*******************************************/

#include"client.h"

#include <QtNetwork/QTcpSocket>
#include <QDebug>

using namespace Werewolf;

//以下是给定不同参数时的构造函数
Client::Client(): _available(false){}

Client::Client(const Client &client): _available(client._available), _sock(client._sock){
}

Client::Client(QTcpSocket *sock): _available(true), _sock(sock){}

Client::~Client(){
}

void Client::set_sock(QTcpSocket *sock){
  _sock = sock;
  _available = true;
}

bool Client::available(){
  return _available;
}

void Client::print(std::string msg){
  QByteArray data = ("PRINT:" + msg + ";").c_str();
  _sock->write(data);
}

void Client::turn_on_input(){
  QByteArray data("TURN_ON;");
  _sock->write(data);
}

void Client::hold_on_input(){
  QByteArray data("HOLD_ON;");
  _sock->write(data);
}

void Client::turn_off_input(){
  _sock->write(QByteArray("TURN_OFF;"));
}

void Client::shut_down(){
  _sock->write(QByteArray("SHUT_DOWN;"));
}


//功能：读取客户端的信息，支持阻塞和非阻塞两种模式
//参数：
//    delay:当delay不为零时，指定非阻塞的延时时间（以秒为单位）；当delay为零时，采用阻塞模式。
//
//返回值：收到的信息
std::string Client::recv(double delay){
  int msec = (int) delay * 1000;

  if(msec == 0){
    if(_sock->waitForReadyRead(-1)){
      QByteArray msg = _sock->readAll();
      return msg.toStdString();
    }
  }

  else{
    if(_sock->waitForReadyRead((int) msec)){
      return _sock->readAll().toStdString();
    }
    else{
      return std::string();
    }
  }

  return std::string();
}

void Client::set_charac(Character *ch){
  _charac = ch;
}


Character* Client::selfCharacter(){
  return _charac;
}

void Client::changename(std::string nickname){
  _nickname = nickname;
}

std::string Client::getnickname(){
  return _nickname;
}
