/*************************************************
名称：processmanager.h
作者：曾军 计65 2016011359
时间：2017/05/10
内容：游戏进程的控制类
版权：完全自行完成
*************************************************/

#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <vector>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "gameprocess.h"
#include "client.h"
#include "character.h"
#include "characterfac.h"


namespace Werewolf{
  class ProcessManager{ 
    std::vector<Process*> _process;				
    std::vector<Client>* _client;
    int ptr;									//进程控制过程中指定进程的指针
	
  public:
	ProcessManager(std::vector<Client>* client) : _client(client){}
    ~ProcessManager(){
      _process.clear();
    }
	
    int calibration();							//由用户决定是屠边还是屠城
    void add(Character* cha, Client* cli);		//将流程核心角色添加到相应的process，方便使用
    void constructlist();						//确定游戏进程
    void Init(Client *cli);						//初始化用户名辅助函数
    void Init();								//初始化用户名
    void shutdown();							//游戏终止函数
    void run();									//运行函数
    void gameover();							//宣布游戏结束并公布游戏结果
  };
}

#endif//PROCESSMANAGER_H
