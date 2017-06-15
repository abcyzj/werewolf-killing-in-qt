/*************************************************
名称：processmanager.cpp
作者：曾军 计65 2016011359
时间：2017/05/10
内容：游戏进程的控制类
版权：完全自行完成
*************************************************/
#include "processmanager.h"
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
using namespace Werewolf;


//功能：用户决定屠边还是屠城
//说明：用户输入数字供之后的判断
int ProcessManager :: calibration(){
  (*_client)[0].print("please choose judge method : 1 for kill one side 2 for kill all");
  (*_client)[0].turn_on_input();
  std::string s = (*_client)[0].recv();
  
  if(s[0] == '1'){
    return (int) 1;
  }
  else if(s[0] == '2'){
    return (int) 2;
  }
  return 1;
}


//功能：构建游戏的进行流程，并将流程串成链表
//说明：填充了process*的vector
void ProcessManager::constructlist(){	
  int msg = calibration();						//获取屠边还是屠城的标准，在构造进程类时传入	
  
  
  Process* pro = new Hunting(_client);
  Process* Pro = new Po_passing(_client);		
  _process.clear();
  
  Process* pro0 = new Guarding(_client);		//构建守卫守人进程
  _process.push_back(pro0);						//添加至vector
  
  Process* pro1 = new Killing(_client);			//构建狼人杀人进程
  _process.push_back(pro1);
  
  Process* pro2 = new Witching(_client);		//构建女巫用药进程
  _process.push_back(pro2);
  
  Process* pro3 = new Predicting(_client);		//构建预言家验身份进程
  _process.push_back(pro3);
  
  Process* pro4 = new Po_electing(_client);		//构建选举警长进程
  _process.push_back(pro4);
  
	Process* pro5 = new Calculating(_client, pro, msg, Pro);
  _process.push_back(pro5);
  
  Process* _pro = new Chat(_client);
  Process* pro6 = new Voting(_client, pro, Pro, _pro,msg);
  
  _process.push_back(pro6);
  _process.push_back(pro);
  _process.push_back(_pro);
  _process.push_back(Pro);
    
	
  for(int i = 0; i < 7; i++){					//将进程串成游戏进行顺序的链表
    if(i != 6)
      _process[i] -> set_next(_process[i + 1]);
    else
      _process[i] -> set_next(_process[0]);
  }
}


//功能：将进程类和进程核心的角色绑定，便于使用
//说明：调用了process类的add_client函数
//参数：
//		cha：角色类的指针
//		cli；客户端的指针
void ProcessManager :: add(Character* cha, Client* cli){
  Werewolf::Killing *klpro;
  switch(cha -> type()){									//根据角色自带参数判断角色，再添加到相应的进程中
  case 1:
    _process[1] -> add_client(cli);
    klpro = dynamic_cast<Werewolf::Killing*>(_process[1]);
    klpro -> push_cli_num(cli - &(_client->at(0)));
    break;
  case 2:
    break;
  case 3:
    _process[7] -> add_client(cli);
    break;
  case 4:
    _process[2] -> add_client(cli);
    break;
  case 5:
    _process[3] -> add_client(cli);
    break;
  case 6:
    _process[0] -> add_client(cli);
    break;
  }
}


//功能：初始化客户端的用户名，提升游戏体验
//参数：
//		cli：指向客户端的指针
void ProcessManager::Init(Client* cli){
  std::cout << "Initing" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  cli -> print("input your nickname");
  cli -> turn_on_input();
  std::string name = cli -> recv();
  cli -> changename(name);
}

//功能：初始化用户名
//说明：是上个函数的重载版本，一次性将所有客户端用户名都初始化
void ProcessManager::Init(){
  std::vector<Client>& client = *_client;
  int size = client.size();
  for(int i = 0;  i < size; i++){
    Init(&client[i]);
  }
}


//功能：结束游戏，退出程序
void ProcessManager :: shutdown(){
  (*_client)[0].print("Press y to shut down the game");
  (*_client)[0].turn_on_input();
  if((*_client)[0].recv() == "y"){
    for(auto i = 0; i < (*_client).size(); i++){
      (*_client)[i].print("exited");
      //(*_client)[i].shut_down();
    }
    exit(0);
  }
}


//功能：游戏结束后宣布游戏结果，并公布相应玩家的身份
//说明：玩家身份由角色类自带的参数确定
void ProcessManager::gameover(){
	
	std::string s;//用于输出内容的string
    for(auto i = 0; i < (*_client).size(); i++){
		int people_rel = (*_client)[i].selfCharacter() -> type();
		if(people_rel == 1){
		  for(auto j = 0; j < (*_client).size(); j++){
			s = std::to_string(i + 1);
					s = "Player " + s + " is werewolf";
					(*_client)[j].print(s);
		  }
		}
		else if(people_rel == 2){
		  for(auto j = 0; j < (*_client).size(); j++){
			s = std::to_string(i + 1);
					s = "Player " + s + " is villager";
					(*_client)[j].print(s);
		  }
		}
		else if(people_rel == 3){
		  for(auto j = 0; j < (*_client).size(); j++){
			s = std::to_string(i + 1);
					s = "Player " + s + " is hunter";
					(*_client)[j].print(s);
		  }
		}
		else if(people_rel == 4){
		  for(auto j = 0; j < (*_client).size(); j++){
			s = std::to_string(i + 1);
					s = "Player " + s + " is witch";
					(*_client)[j].print(s);
		  }
		}
		else if(people_rel == 5){
		  for(auto j = 0; j < (*_client).size(); j++){
			s = std::to_string(i + 1);
					s = "Player " + s + " is prophet";
					(*_client)[j].print(s);
		  }
		}
		else{
		  for(auto j = 0; j < (*_client).size(); j++){
			s = std::to_string(i + 1);
					s = "Player " + s + " is guard";
					(*_client)[j].print(s);
		  }
		}
	  }
	  shutdown();
}


//功能：游戏的运行函数
void ProcessManager :: run(){
  Init();
  for(auto i = 0; i < (*_client).size(); i++){
    (*_client)[i].print("game started!!!");
  }
  for(auto i = 0; i < (*_client).size(); i++){
    std::string s = std::to_string(i + 1);
		s = "You are Player " + s;
		(*_client)[i].print(s);
  }
  
  
  for(int i = 4; i <= 9; i++){				//由于每个进程的运行函数初始设置都是不被激活的，因此，游戏前要激活相应的进程
	  _process[i] -> activate();
  }
  Process* usepro;
  usepro = _process[0];
  for(int i = 0; i < 7; i++){
    int cnt = usepro -> get_size();
    if(usepro -> get_size() != 0){
      usepro -> activate();
    }
    usepro = usepro -> next();
  }
  
  
  while(1)
    for(int i = 0; i < 7; i++){
      if(i <= 4){
        usepro -> begin();
        usepro = usepro -> next();
      }
      else{
        if(!usepro -> begin()){				//当某个进程的运行函数返回false时，说明游戏已经结束							
          ptr = i;							//给确定进程的指针赋值
          gameover();						//这时候退出程序
        }
        else{
          usepro = usepro -> next();
        }
      }
            
    }
}

