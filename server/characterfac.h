#ifndef CHARACTERFAC_H
#define CHARACTERFAC_H

#include <vector>
#include "client.h"
#include "character.h"
#include "gameprocess.h"



namespace Werewolf{
  class ProcessManager;

  class Characterfac{
    std::vector<Client>* _client;
    ProcessManager* _manager;
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


#endif
