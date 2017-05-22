#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <vector>
#include <iostream>
#include "process.h"
#include "client.h"
#include "character.h"
#include <string>
#include "characterfac.h"
#include <stdlib.h>


namespace Werewolf{
    class ProcessManager{
        std::vector<Process*> _process;
        std::vector<Client>* _client;
        int ptr;
    public:
        ProcessManager(std::vector<Client>* client) : _client(client){}
        ~ProcessManager(){
            _process.clear();
        }
        int calibration();
        void add(Character* cha, Client* cli);//将VIP角色添加到相应的process
        void constructlist();//确定游戏进程
        void Init(Client *cli);//初始化用户名辅助函数
        void Init();//初始化用户名
        void shutdown();//游戏终止函数
        void run();//运行函数
        void gameover();//宣布游戏结束并公布游戏结果
    };
}

#endif
