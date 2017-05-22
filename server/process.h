#ifndef PROCESS_H
#define PROCESS_H

#include "character.h"
#include "client.h"
#include <vector>
#include <cstring>
namespace Werewolf
{
    enum Act{BITE=0,POISON=1,SAVE=2,GUARDING=3,SHOOT=4,PREDICT=5,VOTE=6};
    enum Cha{ALL,WOLF,WITCH,GUARD,HUNTER,SEER};
    struct logging
    {
        Cha _doer;
        Act _act;
        int _geter;
    };
    class Process
    {
    protected:
        bool _valid = false;
        Process* _next;
        std::vector<Client*> _rel_cli;
        std::vector<Client>* allclient;
        virtual bool func() = 0;    //进程的主要执行函数
        static std::vector<logging> _log;
    public:
        int get_size();
        Process(std::vector<Client>*);//allclient
        static int have_police;
        virtual bool begin() ;//true继续，false退出
        virtual Process* next();
        virtual void add_client(Client*);
        virtual void set_next(Process*);
        virtual void activate();
        virtual bool valid(); //判断该类所代表进程是否需要进行，需要则调用func函数
        static void deletelog();
        static std::vector<logging>* readlog();
        static void writelog(Cha,Act,int);
        virtual ~Process();
    };
    class Guarding : public Process    //守卫
    {
    protected:
        bool func();
    public:
        Guarding(std::vector<Client> *cli);
    };
    
    class Killing : public Process //狼人杀人
    {
    protected:
        bool func();
		std::vector<int> cli_num;
    public:
        Killing(std::vector<Client> *cli);
        void push_cli_num(int num);
    };
    
    class Witching : public Process//女巫
    {
    protected:
        static int exe_time;    //总执行次数
        bool func();
    public:
        Witching(std::vector<Client> *cli);
        int ex_time();    //总的执行次数
        void add_ex_time(); //执行次数加1
    };
    
    class Predicting : public Process   //预言家
    {
    protected:
        bool func();
    public:
        Predicting(std::vector<Client> *cli);
    };
    
    class Calculating : public Process
    {
    protected:
        Process* _hun;
        Process* _po;
        std::vector<logging>* _log;
        int _calibra;//判断屠边还是屠城的标准，1为屠边，2为屠城
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
            _log = readlog();
        }
        ~Calculating(){
            delete _hun;
        }
    };
    
    class Po_electing : public Process //选举警长
    {
    protected:
        bool func();
    public:
        Po_electing(std::vector<Client> *cli);
    };
    
    class Hunting;
    class Po_passing;
    class Voting : public Process //ͶƱ
    {
    protected:
        bool func();
        Process* ht;
        Process* Po_p;
        Process* _Chat;
        bool is_end();
        int _msg;
    public:
        Voting(std::vector<Client> *cli, Process*, Process*, Process*,int);
    };
    
    class Hunting : public Process
    {
    protected:
        std::vector<Client>* _cli;
        friend class Voting;
        friend class Calculating;
        bool func();
    public:
        Hunting(std::vector<Client> *cli);
    };
    
    class Po_passing:public Process
    {//移交警徽
    protected:
        bool func();
    public:
        friend class Calculating;
        Po_passing(std::vector<Client>*);
    };
    
    class Chat : public Process
    {
    protected:
        static std::vector<logging>* _log;
        std::vector<Client> &client = *allclient;
        int start_one;//存放发言开始之前的那个的人
        bool police=0;//判断有无警长的布尔值
        void read();//读取日志函数
        void right();//向右发言
        void left();//向左发言
        bool func();//行为函数
        int size;
        int dead_num = 0;//存放死的人数量
        int dead_player[100];//存放死的人的序号,记住是从1开始，初始化均为-1
    public:
        Chat(std::vector<Client>*);
    };
}
#endif
