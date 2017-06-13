#include "processmanager.h"
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
using namespace Werewolf;

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
  //(*_client)[0].turn_off_input();
  return 1;
}

void ProcessManager::constructlist(){
  int msg = calibration();
  Process* pro = new Hunting(_client);
  Process* Pro = new Po_passing(_client);
  _process.clear();
  Process* pro0 = new Guarding(_client);
  _process.push_back(pro0);
  Process* pro1 = new Killing(_client);
  _process.push_back(pro1);
  Process* pro2 = new Witching(_client);
  _process.push_back(pro2);
  Process* pro3 = new Predicting(_client);
  _process.push_back(pro3);
  Process* pro4 = new Po_electing(_client);
  _process.push_back(pro4);
	Process* pro5 = new Calculating(_client, pro, msg, Pro);
  _process.push_back(pro5);
  Process* _pro = new Chat(_client);
  Process* pro6 = new Voting(_client, pro, Pro, _pro,msg);
  _process.push_back(pro6);
  _process.push_back(pro);
  _process.push_back(_pro);
  _process.push_back(Pro);
    
  for(int i = 0; i < 7; i++){
    if(i != 6)
      _process[i] -> set_next(_process[i + 1]);
    else
      _process[i] -> set_next(_process[0]);
  }
}



void ProcessManager :: add(Character* cha, Client* cli){
  Werewolf::Killing *klpro;
  switch(cha -> type()){
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

void ProcessManager::Init(Client* cli){
  std::cout << "Initing" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  cli -> print("input your nickname");
  cli -> turn_on_input();
  std::string name = cli -> recv();
  //cli -> turn_off_input();
  cli -> changename(name);
}

void ProcessManager::Init(){
  std::vector<Client>& client = *_client;
  int size = client.size();
  for(int i = 0;  i < size; i++){
    Init(&client[i]);
  }
}


void ProcessManager :: shutdown(){
  (*_client)[0].print("Press y to shut down the game");
  (*_client)[0].turn_on_input();
  if((*_client)[0].recv() == "y"){
    for(auto i = 0; i < (*_client).size(); i++){
      (*_client)[i].print("exited");
      //(*_client)[i].shut_down();
    }
    //(*_client)[0].turn_off_input();
    exit(0);
  }
}

void ProcessManager::gameover(){
  for(auto i = 0; i < (*_client).size(); i++){
    (*_client)[i].print("game over!!!");
  }
	std::string s;
	/*
    if((_process[ptr] -> wolf_lose)){
    for(auto i = 0; i < (*_client).size(); i++){
    (*_client)[i].print("goodman win");
    }
    }
    else{
    for(auto i = 0; i < (*_client).size(); i++){
    (*_client)[i].print("werewolf win");
    }
    }*/
  for(auto i = 0; i < (*_client).size(); i++){
    int people_rel = (*_client)[i].selfCharacter() -> type();
    if(people_rel == 1){
      for(auto j = 0; j < (*_client).size(); j++){
        // (*_client)[i].print( (*_client)[i].getnickname());
        // char str[5];
        //std::to_string(i+1)(i + 1, str, 10);
        s = std::to_string(i + 1);
				s = "Player " + s + " is werewolf";
				(*_client)[j].print(s);
      }
    }
    else if(people_rel == 2){
      for(auto j = 0; j < (*_client).size(); j++){
        // (*_client)[i].print( (*_client)[i].getnickname());
        //char str[5];
        // std::to_string(i+1)(i + 1, str, 10);
        s = std::to_string(i + 1);
				s = "Player " + s + " is villager";
				(*_client)[j].print(s);
      }
    }
    else if(people_rel == 3){
      for(auto j = 0; j < (*_client).size(); j++){
        // (*_client)[i].print( (*_client)[i].getnickname());
        //char str[5];
        //itoa(i + 1, str, 10);
        s = std::to_string(i + 1);
				s = "Player " + s + " is hunter";
				(*_client)[j].print(s);
      }
    }
    else if(people_rel == 4){
      for(auto j = 0; j < (*_client).size(); j++){
        // (*_client)[i].print( (*_client)[i].getnickname());
        //char str[5];
        //itoa(i + 1, str, 10);
        s = std::to_string(i + 1);
				s = "Player " + s + " is witch";
				(*_client)[j].print(s);
      }
    }
    else if(people_rel == 5){
      for(auto j = 0; j < (*_client).size(); j++){
        //  (*_client)[i].print( (*_client)[i].getnickname());
        //char str[5];
        //itoa(i + 1, str, 10);
        s = std::to_string(i + 1);
				s = "Player " + s + " is prophet";
				(*_client)[j].print(s);
      }
    }
    else{
      for(auto j = 0; j < (*_client).size(); j++){
        //(*_client)[i].print( (*_client)[i].getnickname());
        //char str[5];
        //itoa(i + 1, str, 10);
        s = std::to_string(i + 1);
				s = "Player " + s + " is guard";
				(*_client)[j].print(s);
      }
    }
  }
  shutdown();
}

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
  _process[4] -> activate();
  _process[5] -> activate();
  _process[6] -> activate();
  _process[7] -> activate();
  _process[8] -> activate();
  _process[9] -> activate();
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
        if(!usepro -> begin()){
          ptr = i;
          gameover();
        }
        else{
          usepro = usepro -> next();
        }
      }
            
    }
}

