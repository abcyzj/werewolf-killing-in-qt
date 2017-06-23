/*************************************************
名称：process.h
作者：王琛 2016011360 计65
     李映辉 2016011372 计65
     刘应天 2016011358 计65
     曾军 2016011359 计65
时间：2017/05/15
内容：游戏的进程类
版权：完全自行完成
*************************************************/

/*************************************************
 分工情况：
 王琛：
    process基类的构建
    process子类：Guarding,Witching,Predicting,Po_electing
 刘应天：
    logging
    process子类：Killing,Hunting,Po_passing
 李映辉：
    process子类：Chat
 曾军：
    Process子类：Calculating
*************************************************/

/*************************************************
 process类的工作原理：
 process每个子类表示游戏的一个进程，例如女巫用药，狼人杀人等，processmanager负责将每个流程串接起来
 process中的valid表示该进程是否可用，func函数是每个子类最基本的函数，
     进行每项流程的具体操作，而begin则负责开始该进程，如果非valid，直接返回true，如果valid，再去执行func函数
具体的实现见process类及process.cpp
*************************************************/

#ifndef PROCESS_H
#define PROCESS_H


#include <vector>
#include <cstring>
#include "character.h"
#include "client.h"


namespace Werewolf
{
  enum Act{BITE=0,POISON=1,SAVE=2,GUARDING=3,SHOOT=4,PREDICT=5,VOTE=6};//用枚举定义夜晚的操作，便于logging中记录
  enum Cha{ALL,WOLF,WITCH,GUARD,HUNTER,SEER};   //用枚举定义操作的执行者
  //logging功能：记录每晚上发生的操作，写日志
  struct logging
  {
    Cha _doer;  //动作的执行者
    Act _act;   //动作的种类
    int _geter; //动作的承受者
  };
    
  //Process:抽象类，子类表示游戏的每一个流程
  class Process
  {
  protected:
    bool _valid = false;        //表示该进程是否可用
    Process* _next;             //表示该进程的下一个进程
    std::vector<Client*> _rel_cli;  //与该进程相关的client
    std::vector<Client>* allclient; //所有的client构成一个vector
    virtual bool func() = 0;    //进程的主要执行函数
    static std::vector<logging> _log;   //日志的vector，存放当天晚上的情况
  public:
    int get_size();             //得到_rel_cli的大小
    Process(std::vector<Client>*);//构造函数，传入一个vector
    static int have_police;     //表示是否有警长，-1表示没有警长，否则表示玩家编号
    virtual bool begin() ;      //进程开始的函数，根据valid的值作出相应的选择
    virtual Process* next();    //下一个process
    virtual void add_client(Client*);   //加入client
    virtual void set_next(Process*);    //设置下一位client
    virtual void activate();    //激活进程（_valid=true)
    virtual bool valid();       //判断该类所代表进程是否需要进行
    static void deletelog();    //删除日志
    static std::vector<logging>* readlog(); //读日志
    static void writelog(Cha,Act,int);  //写日志
    virtual ~Process();
  };
  
  
  class Guarding : public Process    //守卫的进程
  {
  protected:
    bool func();
  public:
    Guarding(std::vector<Client> *cli);
  };
  
    
  class Killing : public Process //狼人杀人的进程
  {
  protected:
    bool func();
		std::vector<int> cli_num;//存放狼人位置编号，便于按顺序遍历
  public:
    Killing(std::vector<Client> *cli);
    void push_cli_num(int num);//给cli_num添加编号
  };
    
	
  class Witching : public Process   //女巫的进程
  {
  protected:
    static int exe_time;    //总执行次数
    bool func();
  public:
    Witching(std::vector<Client> *cli);
    int ex_time();          //返回总的执行次数
    void add_ex_time();     //执行次数加1
  };
    
	
  class Predicting : public Process   //预言家的进程
  {
  protected:
    bool func();
  public:
    Predicting(std::vector<Client> *cli);
  };
    
	
  class Calculating : public Process    //结算的进程
  {
  protected:
    Process* _hun;
    Process* _po;
    std::vector<logging>* _log;
    int _calibra;				//判断屠边还是屠城的标准，1为屠边，2为屠城
    bool calculatewolf();
    bool calculatepeo();
    bool calculategod();
    bool cal_guard(int);
    bool is_saved(int);
    bool is_poisoned(int);
    bool func();
    void find_dead();
    int officer;
    bool hunting = false;
  public:
  Calculating(std::vector<Client> *cli, Process* hun, int calibra, Process* po) : Process(cli), _hun(hun), _calibra(calibra), _po(po){
      _log = readlog();			//读日志，对当晚发生的事进行结算
    }
    ~Calculating(){
      delete _hun;
    }
  };
    
	
  class Po_electing : public Process //选举警长进程
  {
  protected:
    bool func();
  public:
    Po_electing(std::vector<Client> *cli);
  };
    
	
  class Hunting;
  class Po_passing;
  class Voting : public Process //投票进程
  {
  protected:
    bool func();
    Process* ht;//指向猎人操作进程的指针
    Process* Po_p;//指向移交警徽进程的指针
    Process* _Chat;//指向聊天进程的指针
    bool is_end();//在投票环节结束后游戏是否结束，是-true 否-false
    int _msg;//记录屠边或屠城
  public:
    Voting(std::vector<Client> *cli, Process*, Process*, Process*,int);
  };
   
  class Hunting : public Process //猎人操作的进程
  {
  protected:
    std::vector<Client>* _cli;
    friend class Voting;
    friend class Calculating;
    bool func();
  public:
    Hunting(std::vector<Client> *cli);
  };
    
	
  class Po_passing : public Process //移交警徽的进程
  {
  protected:
    bool func();
  public:
    friend class Calculating;
    Po_passing(std::vector<Client>*);
  };
    
  class Chat : public Process       //Chat类，用于用户之间的聊天功能
  {
  protected:
    static std::vector<logging>* _log;
    std::vector<Client> &client = *allclient;
    int start_one;//存放发言开始之前的那个的人的序号
    bool police=0;//判断有无警长的布尔值
    void read();//读取日志函数
    void right();//向右发言函数
    void left();//向左发言函数
    bool func();//聊天主体行为函数
    int size;//所有玩家数目
    int dead_num = 0;//存放死的人数量
    int dead_player[100];//存放死的人的序号,记住是从1开始，初始化均为-1
  public:
    Chat(std::vector<Client>*);//构造函数
  };
}
#endif//PROCESS_H
