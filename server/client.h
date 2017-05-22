#ifndef CLIENT_H
#define CLIENT_H

#include"character.h"

#include <string>
#include <QtNetwork/QTcpSocket>
#include <QObject>

class QTcpSocket;

namespace Werewolf{
  class Client : public QObject{
    Q_OBJECT
  private:
    bool _available;//指示当前Client是否可用
    std::string _nickname = "UNKOWN";//客户端的昵称
    QTcpSocket *_sock;//和该客户端通信的Socket
    Character* _charac;//该客户端对应的角色类
  public:
    Client();//默认构造函数
    Client(const Client&);//不允许拷贝构造
    //Client& operator = (const Client&) = delete;//不允许拷贝赋值
    //Client& operator = (Client&&);//移动赋值
    Client(QTcpSocket *);//给出Socket的构造函数
    ~Client();
    void set_sock(QTcpSocket *);//Socket也可不在构造时给出
    bool available();//返回当前Client是否可用
    void print(std::string);//命令客户端打印信息
    void turn_on_input();//命令客户端打开输入读取,只读取一次
    void hold_on_input();//命令客户端打开输入读取，直到调用turn_off_input为止
    void turn_off_input();//命令客户端关闭输入读取
    void shut_down();//命令客户端断开连接
    std::string recv(double delay = 0);//从客户端接受信息
    void set_charac(Character*);//绑定角色类
    Character* selfCharacter();
    void changename(std::string);
    std::string getnickname();
  };
}

#endif
