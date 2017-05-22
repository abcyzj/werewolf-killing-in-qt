#include"client.h"

#include <QtNetwork/QTcpSocket>
#include <QDebug>

using namespace Werewolf;

Client::Client(): _available(false){}

Client::Client(const Client &client): _available(client._available), _sock(client._sock){

}

Client::Client(QTcpSocket *sock): _available(true), _sock(sock){}

//Client& Client::operator = (Client &&cl){
//  if(this == &cl)
//    return *this;

//  if(cl.available()){
//    _sock = _sock;
//    _available = true;
//    cl._available = false;
//  }
//  else{
//    _available = fClientalse;
//  }
//  return *this;
//}


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
  qDebug() << "send" << QString(msg.c_str());
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
