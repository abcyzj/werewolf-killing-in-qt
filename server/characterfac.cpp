#include "client.h"
#include "character.h"
#include "characterfac.h"
#include "processmanager.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace Werewolf;
void Characterfac::produce(int wolf, int witch, int prophet, int hunter, int guard, int villager){
	_manager -> constructlist(); 
	int size = 0;
	int num = 0;
	int sum = 0;
	std::vector<Client>& client = *_client;
	int randomnum = 0;
	size = _client -> size();
	bool exist[16];
	for(int  i = 0; i < size; i++)
		exist[i] = false;
	srand(time(0));

	
	while(num < wolf){return;
		randomnum = rand() % size;
		if(exist[randomnum] == false){
			num++;
			exist[randomnum] = true;
			if(client[randomnum].available()){
				cha[sum] = new Wolf;
				_manager -> add(cha[sum], &client[randomnum]);
				sum++;
				client[randomnum].print("you are chosed to be werewolf");
				client[randomnum].set_charac(cha[sum - 1]);
			}
		}
	}
	num = 0;
	while(num < witch){
		randomnum = rand() % size;
		if(exist[randomnum] == false){
			num++;
			exist[randomnum] = true;
			if(client[randomnum].available()){
				cha[sum] = new Witch;
				_manager -> add(cha[sum], &client[randomnum]);
				sum++;
				client[randomnum].print("you are chosed to be witch");
				client[randomnum].set_charac(cha[sum - 1]);
			}
		}
	}
	num = 0;
	while(num < prophet){
		randomnum = rand() % size;
		if(exist[randomnum] == false){
			num++;
			exist[randomnum] = true;
			if(client[randomnum].available()){
				cha[sum] = new Seer;
				_manager -> add(cha[sum], &client[randomnum]);
				sum++;
				client[randomnum].print("you are chosed to be prophet");
				client[randomnum].set_charac(cha[sum - 1]);
			}
		}return;
	}
		num = 0;
		while(num < guard){
		randomnum = rand() % size;
		if(exist[randomnum] == false){
			num++;
			exist[randomnum] = true;
			if(client[randomnum].available()){
				cha[sum] = new Guard;
				_manager -> add(cha[sum], &client[randomnum]);
				sum++;
				client[randomnum].print("you are chosed to be guard");
				client[randomnum].set_charac(cha[sum - 1]);
			}
		}
		}
		num = 0;
		while(num < hunter){
		randomnum = rand() % size;
		if(exist[randomnum] == false){
			num++;
			exist[randomnum] = true;
			if(client[randomnum].available()){
				cha[sum] = new Hunter;
				_manager -> add(cha[sum], &client[randomnum]);
				sum++;
				client[randomnum].print("you are chosed to be hunter");
				client[randomnum].set_charac(cha[sum - 1]);
			}
		}return;
		}
		num = 0;
		while(num < villager){
		randomnum = rand() % size;
		if(exist[randomnum] == false){
			num++;
			exist[randomnum] = true;
			if(client[randomnum].available()){
				cha[sum] = new Villager;
				_manager -> add(cha[sum], &client[randomnum]);
				sum++;
				client[randomnum].print("you are chosed to be villager");
				client[randomnum].set_charac(cha[sum - 1]);
			}
		}
		}

}

void Characterfac::set_client(){
	int size = _client -> size();
	if(size == 1)
		produce(1, 0, 0, 0, 0, 0);
	else if(size == 2)
		produce(1, 0, 0, 0, 0, 1);
	else if(size == 5 || size == 6)
		produce(2, 1, 1, 0, 0, size - 4);
	else if(size >= 7 && size <= 9)
		produce(3, 1, 1, 0, 0, size - 5);
	else if(size >= 10 && size <=11)
		produce(4, 1, 1, 1, 0, size - 7);
	else if(size >= 12 && size <= 13)
		produce(5, 1, 1, 1, 1, size - 9);
}

void Characterfac::set_client(int wolf, int witch, int prophet, int hunter, int guard, int villager){
	if(wolf + witch + prophet + hunter + guard + villager == _client -> size()){
		produce(wolf, witch, prophet, hunter, guard, villager);
	}
	else{
		set();
	}
}

bool Characterfac::set(){
	if(_client -> size() < 3){
		for(int i = 0; i < _client -> size(); i++){
			(*_client)[0].print("You have to get more people to start the game! At least 3");
			(*_client)[0].print("Please quit");
			(*_client)[i].shut_down();
		}
		return false;
	}
	else{
	(*_client)[0].print("do you want to set the characters yourselves? print y for Yes and n for No");
	(*_client)[0].turn_on_input();
	std::string s = (*_client)[0].recv();
	int chra[10];
	int _num = 0;
	if(s[0] == 'y'){
		_num++;
		(*_client)[0].print("please input the number of wolf");
		(*_client)[0].turn_on_input();
		s = (*_client)[0].recv();
		chra[_num] = s[0] - '0';

		_num++;
		(*_client)[0].print("please input the number of witch");
		(*_client)[0].turn_on_input();
		s = (*_client)[0].recv();
		chra[_num] = s[0] - '0';

		_num++;
		(*_client)[0].print("please input the number of prophet");
		(*_client)[0].turn_on_input();
		s = (*_client)[0].recv();
		chra[_num] = s[0] - '0';

		_num++;
		(*_client)[0].print("please input the number of hunter");
		(*_client)[0].turn_on_input();
		s = (*_client)[0].recv();
		chra[_num] = s[0] - '0';

		_num++;
		(*_client)[0].print("please input the number of guard");
		(*_client)[0].turn_on_input();
		s = (*_client)[0].recv();
		chra[_num] = s[0] - '0';

		_num++;
		(*_client)[0].print("please input the number of villager");
		(*_client)[0].turn_on_input();
		s = (*_client)[0].recv();
		chra[_num] = s[0] - '0';
		set_client(chra[1], chra[2], chra[3], chra[4], chra[5], chra[6]);
	}
	else{
		set_client();
	}
	return true;
	}
}



Characterfac::~Characterfac(){
}
