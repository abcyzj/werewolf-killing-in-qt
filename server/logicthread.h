#ifndef LOGICTHREAD_H
#define LOGICTHREAD_H

#include <QThread>
#include <vector>

#include"client.h"

class LogicThread : public QThread
{
public:
  LogicThread(std::vector<Werewolf::Client> *vec);

protected:
  virtual void run();

private:
  std::vector<Werewolf::Client> *clientVec;
};

#endif // LOGICTHREAD_H
