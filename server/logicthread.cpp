#include "logicthread.h"

#include "client.h"
#include "processmanager.h"
#include "characterfac.h"

LogicThread::LogicThread(std::vector<Werewolf::Client> *vec)
  : clientVec(vec)
{

}

void LogicThread::run(){
  Werewolf::ProcessManager manager(clientVec);
  Werewolf::Characterfac fac(clientVec, &manager);
  if(fac.set())
    manager.run();
}
