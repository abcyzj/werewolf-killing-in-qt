/*************************************************
名称：process.cpp
作者：
时间：2017/05/20
内容：游戏的进程类
版权：完全自行完成
*************************************************/
#include "gameprocess.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace Werewolf;

std::vector<logging> Process::_log;
int Process::have_police = -1;
std::vector<logging>* Chat::_log = nullptr;

Process::Process(std::vector<Client>* _all):allclient(_all), _valid(false){
  std::cout<<"All clients have been loaded.\n";
    
}

Process :: ~Process() {}

bool Process :: begin()
{
  if(!valid())
    return true;
  else return func();
}

bool Process :: valid()
{
  return _valid;
}

void Process :: activate()
{
  _valid = 1;
}

int Process::get_size(){
  return _rel_cli.size();
}

Process* Process :: next()
{
  return _next;
}

void Process :: add_client(Client* cli)
{
  _rel_cli.push_back(cli);
}

void Process :: set_next(Process* pro)
{
  _next = pro;
}

void Process :: deletelog()
{
  _log.clear();
}
std :: vector<logging>* Process::readlog(){
  return &_log;
}
void Process::writelog(Cha doer,Act act,int geter)
{
  _log.push_back(logging{doer,act,geter});
}

Killing :: Killing(std::vector<Client> *cli) : Process(cli){}

void Killing :: push_cli_num(int num){
  cli_num.push_back(num);
}

Guarding :: Guarding(std::vector<Client> *cli) : Process(cli){}

Witching :: Witching(std::vector<Client> *cli) : Process(cli){}

Predicting :: Predicting(std::vector<Client> *cli) : Process(cli){}

Po_electing :: Po_electing(std::vector<Client> *cli) : Process(cli){}

Hunting :: Hunting(std::vector<Client> *cli) : Process(cli), _cli(cli){}

int Witching :: exe_time = 0;
int Witching :: ex_time()
{
  return exe_time;
}

void Witching :: add_ex_time()
{
  exe_time++;
}

bool Guarding :: func()
{
  dynamic_cast<Guard*>(_rel_cli[0] -> selfCharacter()) -> who_i_guard(-1);
  if(!_rel_cli[0] -> selfCharacter() -> is_dead())
    {
      _rel_cli[0] -> print("Please input the player number you want to guard(-1 for not guard anyone: \n");
      _rel_cli[0] -> turn_on_input();
      std::string res1 = _rel_cli[0]->recv();
      if (res1 == "-1")
        {
          return true;
        }
      if ((*allclient)[atoi(res1.c_str()) - 1].selfCharacter() -> type() ==
          dynamic_cast<Guard*>(_rel_cli[0] -> selfCharacter()) -> last())
        {
          _rel_cli[0] -> print("You have guarded the same one as last night, please change\n");
          _rel_cli[0] -> print("Please input the player number you want to guard again:\n");
          _rel_cli[0] -> turn_on_input();
          std :: string res2 = _rel_cli[0] -> recv();
          (*allclient)[atoi(res2.c_str())-1].selfCharacter() -> guardfunc();
          dynamic_cast<Guard*>(_rel_cli[0] -> selfCharacter()) -> who_i_guard(atoi(res2.c_str())-1);
          writelog(GUARD, GUARDING, atoi(res2.c_str()) - 1);
        }
      else
        {
          (*allclient)[atoi(res1.c_str())-1].selfCharacter() -> guardfunc();
          dynamic_cast<Guard*>(_rel_cli[0] -> selfCharacter()) -> who_i_guard(atoi(res1.c_str()) - 1);
          writelog(GUARD, GUARDING, atoi(res1.c_str()) - 1);
        }
    }
  return true;
}

bool Killing::func(){
  for(int i=0;i<allclient->size();i++)
    if(!(*allclient)[i].selfCharacter()->is_dead())
      (*allclient)[i].print("Night Falls.\n");
	bool isalive[cli_num.size()];
	int lastalive=-1;
	for(int i=0;i<cli_num.size();i++){
		isalive[i]=!(*allclient)[cli_num[i]].selfCharacter()->is_dead();
		lastalive=cli_num[i];
	}
	for(int i=0;i<cli_num.size();i++)
		if(isalive[i]){
			(*allclient)[cli_num[i]].print("It is wolves' turn.\nPlease chat with your partners to decide which one to kill.\n\tAttention: Player "+std::to_string(lastalive+1)+" can make the decision.\n\tHe can input \"KILL\" when he wants to make the final dicision.\n");
			(*allclient)[cli_num[i]].hold_on_input();
		}
	int tgt=-1;
	while(true){
		for(int i=0;i<cli_num.size();i++)
			if(isalive[i]){
				std::string mes=(*allclient)[cli_num[i]].recv();
				if(mes!=""){
					for(int j=0;j<cli_num.size();j++)
						if(isalive[j])
							(*allclient)[cli_num[j]].print("Player "+std::to_string(cli_num[i]+1)+": "+mes+"\n");
				}
				if(cli_num[i]==lastalive&&mes=="KILL"){
					(*allclient)[lastalive].turn_off_input();
					(*allclient)[lastalive].print("Which one you want to kill?\nJust input the player number.(-1 represents you haven't decided. 0 represents you won't kill anyone)\n");
					(*allclient)[lastalive].turn_on_input();	
					tgt=atoi((*allclient)[lastalive].recv().c_str());
					(*allclient)[lastalive].hold_on_input(); //单机测试的时候注释
				}
			}
		if(tgt>=0)
			break;
	}
	for(int i=0;i<cli_num.size();i++)
		if(isalive[i])
			(*allclient)[cli_num[i]].turn_off_input();
	for(int i=0;i<cli_num.size();i++){
		if(isalive[i]){
			(*allclient)[cli_num[i]].print("A decision has been made.\nChat ends.\n");
			if(tgt>=1)
				(*allclient)[cli_num[i]].print("You will kill Player "+std::to_string(tgt)+".\n");
			else
				(*allclient)[cli_num[i]].print("You won't kill anyone.\n");
		}
	}
	if(tgt>=1)
    writelog(WOLF,BITE,tgt-1);
  return true;
}


bool Witching :: func() //女巫使用毒药或者解药
{
  add_ex_time();
  int w_num = -1, have_dead = -1;  //女巫的client编号和死者的client编号
  for (int i = 0; i < (*allclient).size(); i++)
    {
      if (4 == (*allclient)[i].selfCharacter() -> type())
        w_num = i;
    }
  if (! _rel_cli[0] -> selfCharacter() -> is_dead()) //Â•≥Â∑´Ê≤°Ê≠ª
    {
      int pos_num = dynamic_cast<Witch*>(_rel_cli[0] -> selfCharacter()) -> have_poison();
      int anti_num = dynamic_cast<Witch*>(_rel_cli[0] -> selfCharacter()) -> have_antidote();
      _rel_cli[0] -> print("You have " + std::to_string(pos_num) + " poison " + std :: to_string(anti_num) + " antidote\n");
      if (pos_num == 0 && anti_num == 0) return true;
      int have_dead = -1;
      for(int i = readlog() -> size()-1; i >= 0; i--)
        {
          if((*readlog())[i]._act == BITE)
            {
              have_dead = (*readlog())[i]._geter;
              break;
            }
        }
        
      if (have_dead >= 0)
        {
          _rel_cli[0] -> print("Player " + std::to_string(have_dead + 1) + " have been killed by werewolves\n");
        }
      else _rel_cli[0] -> print("no one dead\n");
        
      //提示使用毒药或者解药
      _rel_cli[0] -> print("Please choose to use poison or antidote, p or a or n for nothing\n");
      _rel_cli[0] -> turn_on_input();
      std::string drug = _rel_cli[0] -> recv();
      if (drug == "n")
        return true;
      while(1)
        {
          if (drug == "p")
            {
              if (pos_num == 0)
                {
                  _rel_cli[0] -> print("ERROR, NO Posion left, please input again\n");
                  _rel_cli[0] -> turn_on_input();
                  drug = _rel_cli[0] -> recv();
                }
              else break;
            }
            
          if (drug == "a")
            {
              if (pos_num == 0)
                {
                  _rel_cli[0] -> print("ERROR, NO antidote left, please input again\n");
                  _rel_cli[0] -> turn_on_input();
                  drug = _rel_cli[0] -> recv();
                }
              if (ex_time() != 1 && have_dead == w_num)
                {
                  _rel_cli[0] -> print("ERROR, You cannot save yourself except the first night\n");
                  _rel_cli[0] -> turn_on_input();
                  drug = _rel_cli[0] -> recv();
                }
              else break;
            }
        }
        
        
      if (drug == "p")    //使用毒药
        {
          _rel_cli[0] -> print("Please Input the player you want to poison\n");
          _rel_cli[0] -> turn_on_input();
          std::string ans = _rel_cli[0] -> recv();
          int num = atoi(ans.c_str());
          writelog(WITCH, POISON, num - 1);
        }
      else
        {
          int have_dead = (*readlog())[readlog() -> size() - 1]._geter;
          writelog(WITCH, SAVE, have_dead);
        }
    }
  return true;
}

bool Predicting :: func()    //预言家进行身份检测
{
  if (! _rel_cli[0] -> selfCharacter() -> is_dead()) //预言家没死
    {
      _rel_cli[0] -> print("Please input the player number you want to see");
      _rel_cli[0] -> turn_on_input();
      std::string player = _rel_cli[0] -> recv();
      int t = (*allclient)[atoi(player.c_str()) - 1].selfCharacter() -> type();
      if (t == 1)
        _rel_cli[0] -> print("The identity of the player is werewolf");
      else
        _rel_cli[0] -> print("The identity of the player is goodman\n");
      writelog(SEER, PREDICT, atoi(player.c_str()) - 1);
    }
  return true;
}

Voting::Voting(std::vector<Client>* _all, Process* hunt,Process* Po_passing ,Process* chat,int msg):Process(_all), ht(hunt),Po_p(Po_passing),_Chat(chat),_msg(msg){}
Po_passing::Po_passing(std::vector<Client>* _all):Process(_all){}
bool Voting::func(){
  _Chat->begin();
  int n=allclient->size();
  bool isalive[n];
  double num[n];
  int voteinfo[n];
  for(int i=0;i<n;i++){
    isalive[i]=!(*allclient)[i].selfCharacter()->is_dead();
    num[i]=0;
    voteinfo[i]=-1;
  }
  std::string voted="You can vote to Player";
  for(int i=0;i<n;i++){
    if(isalive[i])
      voted+=" "+std::to_string(i+1);
  }
  voted+=".\n";
  if(have_police>=0){
    (*allclient)[have_police - 1].print("Please vote a Player!\n"+voted);
    (*allclient)[have_police - 1].turn_on_input();
    std::string pt = (*allclient)[have_police - 1].recv();
    while(!isalive[atoi(pt.c_str())-1])
      {
        (*allclient)[have_police - 1].print("Please choose another Player!, he is dead\n");
        (*allclient)[have_police - 1].turn_on_input();
        pt = (*allclient)[have_police - 1].recv();
      }
    voteinfo[have_police] = atoi(pt.c_str())-1;
    num[atoi(pt.c_str())-1]+=1.5;
  }
  for(int i=0;i<n;i++)
    if(isalive[i] && i!=have_police - 1){
      (*allclient)[i].print("Please vote!\n"+voted);
      if(have_police>=0)
        (*allclient)[i].print("The Police votes to Player "+std::to_string(voteinfo[have_police]+1)+".\n");
      (*allclient)[i].turn_on_input();
      std::string tgt=(*allclient)[i].recv();
      while(!isalive[atoi(tgt.c_str())-1]){
        (*allclient)[i].print("Please choose another Player!, he is dead.\n");
        (*allclient)[i].turn_on_input();
        tgt=(*allclient)[i].recv();
      }
      voteinfo[i]=atoi(tgt.c_str())-1;
      num[atoi(tgt.c_str())-1]+=1.0;
    }
  double maxx=0;
  std::vector<int> maxnum;
  for(int i=0;i<n;i++)
    if(num[i]>maxx)
      maxx=num[i];
  for(int i=0;i<n;i++)
    if(fabs(num[i]-maxx)<1e-9)
      maxnum.push_back(i);
  std::string s="";
  for(int i=0;i<n;i++)
    if(isalive[i])
      s+="Player "+std::to_string(i+1)+" votes to Player "+std::to_string(voteinfo[i]+1)+".\n";
    
  for(int i=0;i<n;i++)
    if(isalive[i])
      (*allclient)[i].print(s);
    
  std::vector<int> deadnum;
  if(maxnum.size()==1){
    (*allclient)[maxnum[0]].selfCharacter()->set_dead();
    isalive[maxnum[0]]=0;
    writelog(ALL,VOTE,maxnum[0]);
    deadnum.push_back(maxnum[0]);
  }
  else{
    std::string tie="Player";
    for(int i=0;i<maxnum.size();i++)
      tie+=" "+std::to_string(maxnum[i]+1);
    tie+=" have the same number of votes.\nRound 2 Chat begins.\nAt the following order:\n\tPlayer";
    int r1=rand()%maxnum.size();
    int r2=rand()%2;
    int order=0;
    if(r2==0)
      order=1;
    else
      order=-1;
    std::vector<int> chod;
    for(int i=0;i<maxnum.size();i++)
      chod.push_back(maxnum[(r1+i*order+maxnum.size())%maxnum.size()]);
    for(int i=0;i<maxnum.size();i++)
      tie+=" "+std::to_string(chod[i]+1);
    for(int i=0;i<n;i++)
      (*allclient)[i].print(tie);
    for(int i=0;i<chod.size();i++){
      for(int j=0;j<n;j++)
        if(isalive[j])
          (*allclient)[j].print("Player "+ std::to_string(chod[i]+1)+"'s turn:\n");
      (*allclient)[chod[i]].print("Please input the words you want to say.\n:q + Enter represents ending.\n");
      std::string saying="";
      while(true){
        (*allclient)[chod[i]].turn_on_input();
        saying=(*allclient)[chod[i]].recv();
        if(saying!=":q")
          for(int j=0;j<n;j++)
            if(isalive[j]&&j!=chod[i])
              (*allclient)[j].print(saying);
            else break;
      }
      for(int j=0;j<n;j++)
        (*allclient)[j].print("Player "+std::to_string(chod[i]+1)+"has finished.\n");
    }
    for(int i=0;i<n;i++)
      (*allclient)[i].print("Round 2 Chat end.\nRound 2 Voting start.\n");
    int num2[n];
    int voteinfo2[n];
    int maxx=0;
    bool canvote[n];
    for(int i=0;i<n;i++){
      num2[i]=0;
      voteinfo2[i]=-1;
      canvote[i]=1;
    }
    for(int i=0;i<maxnum.size();i++){
      canvote[maxnum[i]]=0;
    }
    std::string sec="You can only vote to Player";
    for(int i=0;i<maxnum.size();i++)
      sec+=" "+std::to_string(maxnum[i]+1);
    sec+=".\n";
    maxnum.clear();
    for(int i=0;i<n;i++){
      if(isalive[i]&&canvote[i]){
        (*allclient)[i].print("Please vote!\n"+sec);
        (*allclient)[i].turn_on_input();
        std::string st=(*allclient)[i].recv();
        while(canvote[atoi(st.c_str())-1]){
          (*allclient)[i].print("You cannot vote that Player.\nPlease choose another Player.\n");
          st=(*allclient)[i].recv();
        }
        voteinfo2[i]=atoi(st.c_str())-1;
        num2[atoi(st.c_str())-1]++;
      }
    }
    for(int i=0;i<n;i++)
      if(num2[i]>maxx)
        maxx=num2[i];
    for(int i=0;i<n;i++)
      if(num2[i]==maxx)
        maxnum.push_back(i);
    std::string secs="";
    for(int i=0;i<n;i++)
      if(isalive[i]&&canvote[i])
        secs+="Player "+std::to_string(i+1)+" votes to Player "+std::to_string(voteinfo2[i]+1)+".\n";
    for(int i=0;i<n;i++)
      if(isalive[i])
        (*allclient)[i].print(s);
    for(int i=0;i<maxnum.size();i++){
      (*allclient)[maxnum[i]].selfCharacter()->set_dead();
      //	isalive[maxnum[i]]=0;
      writelog(ALL,VOTE,maxnum[i]);
      deadnum.push_back(maxnum[i]);
    }
  }
  std::string deathinfo="Player";
  for(int i=0;i<deadnum.size();i++)
    deathinfo+=" "+std::to_string(deadnum[i]+1);
  deathinfo+=" out!\n";
  for(int i=0;i<n;i++)
    if(isalive[i])
      (*allclient)[i].print(deathinfo);
  bool isend=is_end();
  if(isend)
    return false;
  else{
    bool flag1=0;
    bool flag2=0;
    for(int i=0;i<deadnum.size();i++)
      if(deadnum[i]==have_police){
        flag1=1;
        break;
      }
    if(flag1)
      Po_p->begin();
        
    for(int i=0;i<deadnum.size();i++)
      if((*allclient)[deadnum[i]].selfCharacter()->type()==hunter){
        flag2=1;
        break;
      }
    if(flag2){
      ht->begin();
      bool isend2=is_end();
      if(isend2)
        return false;
      else{
        if((*readlog())[readlog()->size()-1]._act==SHOOT){
          int behunt=(*readlog())[readlog()->size()-1]._geter;
          if(behunt==have_police)
            Po_p->begin();
        }
      }
    }
    deadnum.clear();
    for(int i=_log.size()-1;i>=0;i--)
      if(_log[i]._act==VOTE||_log[i]._act==SHOOT)
        deadnum.push_back(_log[i]._geter);
      else
        break;
    for(int i=0;i<n;i++)
      isalive[i]=!(*allclient)[i].selfCharacter()->is_dead();
    for(int i=0;i<deadnum.size();i++)
      isalive[deadnum[i]]=1;
    for(int i=deadnum.size()-1;i>=0;i--){
      (*allclient)[deadnum[i]].print("Please input your last words.\n:q + Enter represents ending.\n");
      (*allclient)[deadnum[i]].turn_on_input();
      std::string lword="Player "+std::to_string(deadnum[i]+1)+"'s last wotds start:\n";
      std::string ladd="";
      ladd=(*allclient)[deadnum[i]].recv();
      while(ladd!=":q"){
        lword+=ladd+"\n";
        (*allclient)[deadnum[i]].turn_on_input();
        ladd=(*allclient)[deadnum[i]].recv();
      }
      lword+="Last words end.\n";
      for(int j=0;j<n;j++)
        if(isalive[j]&&j!=deadnum[i])
          (*allclient)[j].print(lword);
      (*allclient)[deadnum[i]].print("You quit the game!\n");
      isalive[deadnum[i]]=0;
    }
  }
  return true;
}

bool Voting::is_end(){
  int wolfnum=0;
  int villnum=0;
  int godnum=0;
  int n=allclient->size();
  for(int i=0;i<n;i++)
    if(!(*allclient)[i].selfCharacter()->is_dead()){
      int tp=(*allclient)[i].selfCharacter()->type();
      if(tp==wolf)
        wolfnum++;
      else if(tp==villager)
        villnum++;
      else godnum++;
    }
  if(_msg==1){
    if(wolfnum==0&&villnum!=0&&godnum!=0){
      for(int i=0;i<n;i++)
        (*allclient)[i].print("Good Man Win!\n");
      return true;
    }
    if(wolfnum==0&&(villnum==0||godnum==0)){
      for(int i=0;i<n;i++)
        (*allclient)[i].print("Tie Game!\n");
      return true;
    }
    if(wolfnum!=0&&(villnum==0||godnum==0)){
      for(int i=0;i<n;i++)
        (*allclient)[i].print("Wolf Win!\n");
    }
    if(wolfnum!=0&&villnum!=0&&godnum!=0)
      return false;
  }
  else{
    int goodman=godnum+villnum;
    if(goodman==0&&wolfnum!=0){
      for(int i=0;i<n;i++)
        (*allclient)[i].print("Wolf Win!\n");
      return true;
    }
    if(goodman==0&&wolfnum==0){
      for(int i=0;i<n;i++)
        (*allclient)[i].print("Tie Game!\n");
      return true;
    }
    if(goodman!=0&&wolfnum==0){
      for(int i=0;i<n;i++)
        (*allclient)[i].print("Good Man Win!\n");
      return true;
    }
    if(goodman!=0&&wolfnum!=0)
      return false;
  }
  return false;
}

//功能：找到晚上被狼人杀死的人
//说明：通过读日志实现
void Calculating::find_dead(){
  for(int j = 0; j < (*_log).size(); j++){
    if((*_log)[j]._act == 0){
      (*allclient)[(*_log)[j]._geter].selfCharacter() -> set_dead();
    }
  }
}

//功能：判断角色是否被守卫
//参数：
//		cnt：玩家的序号
bool Calculating::cal_guard(int cnt)
{
  return (*allclient)[cnt].selfCharacter()->is_guarded();
}

//功能：判断角色是否被女巫救
//参数：
//		cnt：玩家的序号
bool Calculating::is_saved(int cnt){
  for(int i = 0; i < (*_log).size(); i++){
    if((*_log)[i]._act == 2){
      if((*_log)[i]._geter == cnt){
        return true;
      }
    }
  }
  return false;
}

//功能：判断角色是否被女巫毒
//参数：
//		cnt：玩家的序号
bool Calculating::is_poisoned(int cnt){
  for(int i = 0; i < (*_log).size(); i++){
    if((*_log)[i]._act == 1){
      if((*_log)[i]._geter == cnt){
        return true;
      }
    }
  }
  return false;
}


//功能：判断狼人是否全部被杀死
//返回值：
//		若全部被杀死，则返回true
bool Calculating::calculatewolf(){
  int sum = 0;										     //sum, num 用于计数			
  int num = 0;
  std::vector<Client>& m = *allclient;
  for(auto i = 0; i < m.size(); i++){
    if(m[i].selfCharacter() -> type() == 1){
      sum++;
      if(m[i].selfCharacter() -> is_dead() == true){	//被狼人杀
        if(cal_guard(i) && is_saved(i)){				//同守同救，依然看做被杀死
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          if(m[i].selfCharacter() -> type() == 3){
            hunting = true;								//死的是猎人，在后续进程中可以开枪
          }
          if(m[i].selfCharacter() -> is_police()){
            officer = i;								//死的是警长，选择飞警徽和撕警徽
          }
                    
        }
        else if(cal_guard(i)){
          m[i].selfCharacter() -> set_alive();			//被守卫守，女巫救则依然是活的
        }
        else if(is_saved(i)){
          m[i].selfCharacter() -> set_alive();
        }
        else{
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          if(m[i].selfCharacter() -> is_police()){
            officer = i;
          }
    
        }
      }
      else{												//若没有被杀，则判断有没有被女巫毒
        if(is_poisoned(i)){
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          m[i].selfCharacter() -> set_dead();
          if(m[i].selfCharacter() -> type() == 3){
            hunting = true;
          }
          if(m[i].selfCharacter() -> is_police()){
            officer = i;
          }
          }
      }
    }
  }
  if(num == sum){
    return true;
  }
  return false;
    
}


//功能：判断村民是否全部被杀死
//返回值：
//		若全部被杀死，则返回true
//说明：和上述函数过程相似
bool Calculating::calculatepeo(){
  int sum = 0;
  int num = 0;
  std::vector<Client>& m = *allclient;
  for(int i = 0; i < m.size(); i++){
    if(m[i].selfCharacter() -> type() == 2){
      sum++;
      if(m[i].selfCharacter() -> is_dead() == true){
        if(cal_guard(i) && is_saved(i)){
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          if(m[i].selfCharacter() -> is_police()){
            officer = i;
          }
        }
        else if(cal_guard(i)){
          m[i].selfCharacter() -> set_alive();
        }
        else if(is_saved(i)){
          m[i].selfCharacter() -> set_alive();
        }
        else{
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          if(m[i].selfCharacter() -> type() == 3){
            hunting = true;
          }
          if(m[i].selfCharacter() -> is_police()){
            officer = i;
          }
        }
      }
      else{
        if(is_poisoned(i)){
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          m[i].selfCharacter() -> set_dead();
          if(m[i].selfCharacter() -> type() == 3){
            hunting = true;
          }
          if(m[i].selfCharacter() -> is_police()){
            officer = i;
          }
        }
      }
    }
  }
  if(num == sum){
    return true;
  }
  return false;
}


//功能：判断神民是否全部被杀死
//返回值：
//		若全部被杀死，则返回true
//		反之，返回false
//说明：和上述函数过程相似
bool Calculating::calculategod(){
  int sum = 0;
  int num = 0;
  std::vector<Client>& m = *allclient;
  for(auto i = 0; i < m.size(); i++){
    if((m[i].selfCharacter() -> type() != 1 )&&(m[i].selfCharacter() -> type() != 2)){
      sum++;
      if(m[i].selfCharacter() -> is_dead() == true){
        if(cal_guard(i) && is_saved(i)){
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          if(m[i].selfCharacter() -> type() == 3){
            hunting = true;
          }
          if(m[i].selfCharacter() -> is_police()){
            officer = i;
          }
        }
        else if(cal_guard(i)){
          m[i].selfCharacter() -> set_alive();
        }
        else if(is_saved(i)){
          m[i].selfCharacter() -> set_alive();
        }
        else{
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          if(m[i].selfCharacter() -> type() == 3){
            hunting = true;
          }
                    
          if(m[i].selfCharacter() -> is_police()){
            officer = i;
          }
        }
      }
      else{
        if(is_poisoned(i)){
          num++;
          for(int j = 0; j < allclient -> size(); j++){
            std::string s = std::to_string(i + 1);
            s = "Player " + s + " is dead";
            (*allclient)[j].print(s);
          }
          m[i].selfCharacter() -> set_dead();
          if(m[i].selfCharacter() -> type() == 3){
            hunting = true;
          }
          if(m[i].selfCharacter() -> is_police()){
            officer = i;
          }
        }
      }
    }
  }
  if(num == sum){
    return true;
  }
  return false;
}


//功能：结算类的运行函数
//返回值：
//		若满足游戏结束条件，返回false
//		反之，返回true
bool Calculating::func(){
  find_dead();
  if(_calibra == 1){											//标准为屠边
    if(calculategod() || calculatepeo()){
      for(int i = 0; i < allclient -> size(); i++)
        (*allclient)[i].print("Wolf Win!");
      return false;
    }
    else if(calculatewolf()){
      for(int i = 0; i < allclient -> size(); i++)
        (*allclient)[i].print("Good Man Win!");
      return false;
    }
    else{
      if((*allclient)[officer].selfCharacter() -> is_police()){	//游戏未结束，执行警长，猎人死亡后的操作
        _po -> begin();
      }
      if(hunting){
        _hun -> begin();
      }
	  return true;
    }
        
  }
  else{
    if(calculategod() && calculatepeo()){						//标准为屠城
      for(int i = 0; i < allclient -> size(); i++)
        (*allclient)[i].print("Wolf Win!");
      return false;
    }
    else if(calculatewolf()){
      for(int i=0;i<allclient->size();i++)
        (*allclient)[i].print("Good Man Win!");
      return false;
    }
    else{
      if((*allclient)[officer].selfCharacter() -> is_police()){	//游戏未结束，执行警长，猎人死亡后的操作
        _po -> begin();
      }
      if(hunting){
        _hun -> begin();
      }
      return true;
    }
  }
}


bool check(int* a, int size, int k) //检查数组a中有没有元素k
{
  for (int i = 0; i < size; i++)
    {
      if (a[i] == k)
        return true;
    }
  return false;
}

bool Po_electing :: func()  //选举警长
{
  have_police = -1; //-1表示没有警长，否则表示玩家编号
  int num[(*allclient).size() + 1], cnt = 0;   //num中存竞选警长的人的编号，cnt表示竞选警长的人的个数
  int tot_poll[(*allclient).size() + 1];  //记录每个竞选警长的玩家所得票数，与num相对应
  for (int i = 0; i < (*allclient).size(); i++)
    {
      tot_poll[i] = 0;
      num[i] = 0;
    }
  int max_poll = 0;   //表示最大票数，防止有重复要重新计票
  for (int i = 0; i < (*allclient).size(); i++)   //遍历询问想当警长的人
    {
      (*allclient)[i].print("Do you want to be police, y or n?\n");
      (*allclient)[i].turn_on_input();
      std::string ans = (*allclient)[i].recv();
      if (ans[0] == 'y')
        num[cnt++] = i + 1; //记录下想当警长的人的编号
    }
    
  if (cnt == 0)
    return true;
  for (int i = 0; i < cnt; i++)
    {
      if (num[i] != 0)
        {
          (*allclient)[num[i] - 1].print("Please input your Declaration of candidacy\n");
          (*allclient)[num[i] - 1].turn_on_input();
          std::string words = (*allclient)[num[i] - 1].recv(); //接受竞选宣言
          for (int j = 0; j < (*allclient).size(); j++)//将竞选宣言发给所有玩家
            {
              (*allclient)[j].print("words from player " + std::to_string(num[i]) + " is: " + words);
            }
            
          for (int j = 0; j < cnt; j++)    //每位竞选者发言后询问是否有人退出竞选
            {
              if (num[j] != 0)
                {
                  (*allclient)[num[j] - 1].print("Do you want to quit electing, y or n");
                  (*allclient)[num[j] - 1].turn_on_input();
                  std::string ans = (*allclient)[num[j] - 1].recv();   //得到信息是否退水
                  if (ans[0] == 'y')
                    num[j] = 0;  //退水则将编号置为0
                }
            }
        }
    }
    
  int fin_cnt = 0, fin_rank = 0; //计算退水后竞选警长的人数
  for (int i = 0; i < cnt; i++)
    {
      if (num[i] != 0)
        {
          fin_cnt++;
          fin_rank = num[i];
        }
    }
  if (fin_cnt == 0)
    {
      for (int i = 0; i < (*allclient).size(); i++)//将竞选宣言发给所有玩家
        {
          (*allclient)[i].print("No one want to police\n");
        }
      return true;  //退水后无人竞选
    }
  if (fin_cnt == 1)   //  退水后只剩下一个人，直接设为警长
    {
      for (int i = 0; i < (*allclient).size(); i++)//将竞选宣言发给所有玩家
        {
          (*allclient)[i].print("Only player " + std::to_string(fin_cnt) + " wants to be police\n");
        }
      have_police = fin_rank;
      return true;
    }
  if (fin_cnt == (*allclient).size()) //退水后竞选的人还是所有人，直接没有警长
    {
      for (int i = 0; i < (*allclient).size(); i++)//将竞选宣言发给所有玩家
        {
          (*allclient)[i].print("There is no police since you all want to be police\n");
        }
      have_police = -1;
      return true;
    }
    
  for (int i = 0; i < (*allclient).size(); i++)
    {
      if (!check(num, cnt, i + 1))  //没有死且不参加竞选
        {
          for (int j = 0; j < cnt; j++)
            {
              if (num[j])
                (*allclient)[i].print("Player involved in election include " + std::to_string(num[j]));
            }
          (*allclient)[i].print("Please input the player you want to choose to be the police\n");
          (*allclient)[i].turn_on_input();
          std::string ans = (*allclient)[i].recv();
          for (int l = 0; l < cnt; l++)
            {
              if (num[l] == atoi(ans.c_str()))    //找到相应的人并计票
                {
                  tot_poll[l] += 1;
                  break;
                }
            }
        }
    }
  int target = 0; //记录最大票数的人
  //票数相同再次发言并重新投票
  for (int i = 0; i < cnt; i++)   //开始寻找最大票数
    {
      if (tot_poll[i] > max_poll)
        {
          max_poll = tot_poll[i];
          target = num[i];
        }
    }
  // for (int i = 0; i < cnt; i++)
  //{
  //  std::cout << num[i] << " " << tot_poll[i] << std::endl;
  //}
  std::vector<int> rep;    //计算有最大票数的人数
  for (int i = 0; i < cnt; i++)
    {
      if (tot_poll[i] == max_poll)
        {
          //std::cout << "player " << num[i] << "is max";
          rep.push_back(num[i]);
          //std::cout << std::endl;
        }
    }
  if(rep.size() == 1)    have_police = target;   //警长
  else    //有多人同票
    {
      for (int i = 0; i < (*allclient).size(); i++) //向每个人发送消息
        {
          for (int j = 0; j < rep.size(); j++)
            {
              (*allclient)[i].print("Player " + std::to_string(rep[j]) + " have the same poll, please say and vote again");
            }
        }
      for (int i = 0; i < rep.size(); i++) //同票者重新发言
        {
          (*allclient)[rep[i] - 1].print("Please input your declaration again\n");
          (*allclient)[rep[i] - 1].turn_on_input();
          std::string words = (*allclient)[rep[i] - 1].recv();
          for (int j = 0; j < (*allclient).size(); j++)
            {
              (*allclient)[j].print("words from Player " + std::to_string(rep[i]) + " is: " + words);
            }
        }
      //   for (int i = 0; i < cnt; i++)
      //       std::cout << "player still " << num[i] << std::endl;
      for (int i = 0; i < cnt; i++)
        {
          if (tot_poll[i] != max_poll)
            num[i] = 0;
        }
        
      for (int i = 0; i < cnt; i++)
        tot_poll[i] = 0;    //票数清空
        
      // for (int i = 0; i < cnt; i++)
      //      std::cout << "player still " << num[i] << std::endl;
      for (int i = 0; i < (*allclient).size(); i++)   //重新计票
        {
          if (!check(num, cnt, i + 1))  //没有死且不参加竞选
            {
                
              (*allclient)[i].print("Please input the player you want to choose to be the police again\n");
              (*allclient)[i].turn_on_input();
              std::string ans = (*allclient)[i].recv();
              for (int l = 0; l < cnt; l++)
                {
                  if (num[l] == atoi(ans.c_str()))    //找到相应的人并计票
                    {
                      tot_poll[l] += 1;
                      break;
                    }
                }
                
            }
        }//重新计票结束
        
      max_poll = 0, target = 0;
      for (int i = 0; i < cnt; i++)   //再次开始寻找最大票数
        {
          if (tot_poll[i] > max_poll)
            {
              max_poll = tot_poll[i];
              target = num[i];
            }
        }
      std::vector<int> rep2;    //计算有最大票数的人数
      for (int i = 0; i < cnt; i++)
        {
          if (tot_poll[i] == max_poll)
            rep2.push_back(num[i]);
        }
      if(rep2.size() == 1)    have_police = target;   //警长
      else have_police = -1;
    }
    
  _valid = false; //执行完一次后直接不再可用（只竞选一次警长）
  return true;
}

bool Po_passing::func(){
  int n=allclient->size();
  bool isalive[n];
  for(int i=0;i<n;i++)
    isalive[i]=!(*allclient)[i].selfCharacter()->is_dead();
  for(int i=_log.size()-1;i>=0;i--){
    if(_log[i]._act==VOTE)
      isalive[_log[i]._geter]=1;
    else break;
  }
  for(int i=0;i<n;i++)
    if(isalive[i])
      (*allclient)[i].print("The Police Out!\n");
    
  (*allclient)[have_police].print("Please choose to pass the police to someone or not.(Y/N)\n");
  (*allclient)[have_police].turn_on_input();
  std::string pp="";
  pp=(*allclient)[have_police].recv();
  while(pp[0]!='Y'&&pp[0]!='N'){
    (*allclient)[have_police].print("Input again.\n");
    (*allclient)[have_police].turn_on_input();
    pp=(*allclient)[have_police].recv();
  }
  if(pp[0]=='N'){
    for(int i=0;i<n;i++)
      if(isalive[i])
        (*allclient)[i].print("The Police chooses to tear the pooolice sign.\n");
    have_police=-1;
    _rel_cli.clear();
  }
  else{
    (*allclient)[have_police].print("Please choose a Player to be the next Police.\n\tJust input player number.\n");
    std::string pass="";
    (*allclient)[have_police].turn_on_input();
    pass=(*allclient)[have_police].recv();
    while((*allclient)[atoi(pass.c_str())-1].selfCharacter()->is_dead()){
      (*allclient)[have_police].print("That Player is dead.\nPlease choose another Player.\n");
      pass=(*allclient)[have_police].recv();
    }
    for(int i=0;i<n;i++)
      if(isalive[i])
        (*allclient)[i].print("The Police chooses Player "+std::to_string(atoi(pass.c_str())-1)+" to be the next Police.\n");
    have_police=atoi(pass.c_str());
    _rel_cli.clear();
    _rel_cli.push_back(&((*allclient)[have_police]));
  }
  return true;
}

Chat :: Chat(std::vector<Client>* _cli) : Process(_cli)
{
  size = _cli -> size();
}

void Chat :: read()//Ëé∑Âèñdead_num,dead_player[],
{
    
  int bite_man = -1;
  int poison_man = -1;
  int save_man = -1;
  int guard_man = -1;
  memset(dead_player, -1, sizeof(dead_player));
  _log = readlog();
  for(int i=0 ; i<_log -> size() ; i++)//Âà§Êñ≠ÊúâÊ≤°ÊúâË¢´Âí¨ÊàñËÄÖË¢´ÊØí,Ë¢´ÂÆàÂç´ÔºåË¢´Êïë
    {
      if((*_log)[i]._act == BITE)
        bite_man = (*_log)[i]._geter;
      if((*_log)[i]._act == POISON)
        poison_man = (*_log)[i]._geter;
      if((*_log)[i]._act == SAVE)
        save_man = (*_log)[i]._geter;
      if((*_log)[i]._act == GUARDING)
        guard_man = (*_log)[i]._geter;
    }
  for(int i=0 ; i<size ; i++)//Âà§Êñ≠ÊúâÊ≤°ÊúâË¢´ÂÆàÂç´ÊàñËÄÖË¢´Êïë
    {
      if((bite_man == i)&&(save_man != i)&&(guard_man != i))//Ë¢´Âí¨Ôºå‰∏çË¢´ÊïëÔºå‰∏çË¢´ÂÆàÂç´
        {
          dead_num++;
          dead_player[dead_num]=i;
        }
      else if((poison_man == i)&&(guard_man != i)) //  Ë¢´ÊØíÔºå‰∏çË¢´ÂÆàÂç´
        {
          dead_num++;
          dead_player[dead_num]=i;
        }
      else if((bite_man == i)&&(save_man == i)&&(guard_man == i))//Ë¢´Âí¨ÔºåÂêåÂÆàÂêåÊïë
        {
          dead_num++;
          dead_player[dead_num]=i;
        }
    }
}

void Chat :: right()
{
  read();
  for(int m=start_one+1; m < size; m++)
    {
      if(! client[m].selfCharacter() -> is_dead())
        {
          client[m].print("Please input your massages:");
          client[m].turn_on_input();
          std::string s = client[m].recv();
          for(int k = 0; k < size; k++)
            {
              if(! client[k].selfCharacter() -> is_dead())
                {
                  client[k].print("this is player " + std::to_string(m + 1) + "\n");
                  client[k].print(s);
                }
            }
        }
    }
  for(int m = 0 ; m <=start_one ; m++)
    {
      if(! client[m].selfCharacter() -> is_dead())
        {
          client[m].print("Please input your massages:");
          client[m].turn_on_input();
          std::string s = client[m].recv();
          for(int k = 0; k < size; k++)
            {
              if(! client[k].selfCharacter() -> is_dead())
                {
                  client[k].print("this is player " + std::to_string(m + 1) + "\n");
                  client[k].print(s);
                }
            }
        }
    }
}

void Chat :: left()
{
  read();
  int size = client.size();
  if(start_one==0)
    {
      for(int m = size-1; m >= 0; m--)
        {
          if(! client[m].selfCharacter() -> is_dead())
            {
              client[m].print("Please input your massages:");
              client[m].turn_on_input();
              std::string s = client[m].recv();
              for(int k = 0; k < size; k++)
                {
                  if(! client[k].selfCharacter() -> is_dead())
                    {
                      client[k].print("this is player " + std::to_string(m + 1) + "\n");
                      client[k].print(s);
                    }
                }
            }
        }
        
    }
  else
    {
      for(int m = start_one-1; m>=0 ; m--)
        {
          if(! client[m].selfCharacter() -> is_dead())
            {
              client[m].print("Please input your massages:");
              client[m].turn_on_input();
              std::string s = client[m].recv();
              for(int k = 0; k < size; k++)
                {
                  if(! client[k].selfCharacter() -> is_dead())
                    {
                      client[k].print("this is player " + std::to_string(m + 1) + "\n");
                      client[k].print(s);
                    }
                }
            }
        }
      for(int m = size-1; m >=start_one ; m--)
        {
          if(! client[m].selfCharacter() -> is_dead())
            {
              client[m].print("Please input your massages:");
              client[m].turn_on_input();
              std::string s = client[m].recv();
              for(int k = 0; k < size; k++)
                {
                  if(! client[k].selfCharacter() -> is_dead())
                    {
                      client[k].print("this is player " + std::to_string(m + 1) + "\n");
                      client[k].print(s);
                    }
                }
            }
        }
    }
}


bool Chat :: func()
{
  for(int i = 0; i < (*allclient).size(); i++)
    if(!(*allclient)[i].selfCharacter() -> is_dead())
      (*allclient)[i].print("Daytime begins.\n");
  read();
  if (dead_num == 0)
    {
      for(int i = 0; i < size ; i++)
        {
          if(!(*allclient)[i].selfCharacter() -> is_dead())
            (*allclient)[i].print("Silent Night!");
        }
    }
  else
    {
      std::string dead_peo = "Player";
      for(int i = 1; i <= dead_num; i++)
        {
          dead_peo+=" "+std::to_string(dead_player[i] + 1);
        }
      dead_peo += " have died.\n";
      for(int i = 0; i<size ; i++)
        {
          if(!(*allclient)[i].selfCharacter() -> is_dead())
            (*allclient)[i].print(dead_peo);
        }
        
    }
  for(int i=0 ; i < size ; i++)
    {
      for(int j=1 ; j<=dead_num ; j++)
        {
          if(dead_player[j] == i)
            {
              client[i].print("Please input your last words:");
              client[i].turn_on_input();
              std::string s = client[i].recv();
              for(int k = 0; k<size ; k++)
                client[k].print(s);
            }
        }
    }
  for(int i=0 ; i < size ; i++)
    {
      //std::cout << have_police << std::endl;
      if(have_police == i+1 )
        {
          police=1;
          if (dead_num==1)
            {
              start_one=dead_player[1];
              client[i].print("Please choose left or right:");
              client[i].turn_on_input();;
              std::string p = client[i].recv();
              if(p[0] == 'r')
                right();
              else
                left();
            }
          else
            {
              start_one=i;
              client[i].print("Please choose left or right:");
              client[i].turn_on_input();
              std::string p = client[i].recv();
              if(p[0] == 'r')
                right();
              else
                left();
            }
        }
      else
        continue;
    }
  if(police==0)
    {
      if(dead_num==1)
        {
          start_one = dead_player[1];
          srand( (unsigned)time( NULL ) );
          int left_right = rand() % 2;
          if(left_right==1)
            right();
          else
            left();
        }
      else
        {
          srand( (unsigned)time( NULL ) );
          int t =  rand() % size + 1;
          start_one = dead_player[t];
          int left_right = rand() % 2;
          if(left_right==1)
            right();
          else
            left();
        }
        
    }
  return true;
}

bool Hunting :: func()
{
  std::vector <Client>& tep_cli = *_cli;
  for(int i = 0 ; i < tep_cli.size() ; i++)
    {
      if(tep_cli[i].selfCharacter() -> type() == 3)
        {
          tep_cli[i].print("Please choose a man you want to kill");
          tep_cli[i].turn_on_input();
          std::string s = tep_cli[i].recv();
          int x = std::atoi(s.c_str());
          tep_cli[x].selfCharacter() ->set_dead();
        }
    }
  return true;
}
