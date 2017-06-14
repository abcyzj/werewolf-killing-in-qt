/*************************************************
名称：characterfac.h
作者：曾军 计65 2016011359
时间：2017/05/03
内容：角色类的工厂类
版权：完全自行完成
*************************************************/

#ifndef CHARACTERFAC_H
#define CHARACTERFAC_H

#include <vector>
#include "client.h"
#include "character.h"
#include "gameprocess.h"



namespace Werewolf{
  class ProcessManager;

  class Characterfac{
    std::vector<Client>* _client;//主程序传入的客户端指针的vector
    ProcessManager* _manager;//需要用到ProcessManager的成员函数
  public:
    Character* cha[16];
	Characterfac(std::vector<Client>* a, ProcessManager* manager) : _client(a), _manager(manager){}
    ~Characterfac();
	
    void produce(int wolf, int witch, int prophet, int hunter, int guard, int villager);
    void set_client();
    void set_client(int wolf, int witch, int prophet, int hunter, int guard, int villager);
    bool set();
  };
}


#endif//CHARACTERFAC_H
