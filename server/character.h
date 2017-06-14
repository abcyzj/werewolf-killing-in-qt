//Name : character.h
//Author : 王琛
//Date : 2017-05-15
//Description : 声明了所有角色的基类和子类
//Copyright : 自行完成

#ifndef CHARACTER_H
#define CHARACTER_H

namespace Werewolf
{
  //使用enum枚举类定义不同角色的编号
  //从1-6分别为狼人，村民，猎人，女巫，预言家，守卫（编号顺序人为规定，不同顺序不影响程序执行）
  enum ch_type {wolf = 1, villager, hunter, witch, seer, guard};
    
  //Character类是所有角色的基类（抽象类），声明了所有角色共有的信息和操作
  class Character
  {
    bool police = 0;    //是否为警长
    bool dead = 0;      //是否死亡
    bool _guard = 0;    //是否被守卫
  public:
    virtual int type() = 0; //纯虚函数，返回一个int值代表角色类型
    bool is_dead();     //是否死亡
    bool is_police();	//是否为警长
    bool is_guarded();  //处于被守护状态
    void guardfunc();   //被守卫
    void set_alive();   //复活（女巫解药）
    void set_dead();    //被杀死
    void set_police();  //设为警长
  };

  //下面六个类为6种角色，除了女巫守卫其他只需要重写type虚函数
  class Wolf : public Character
  {
  public:
    int type();
  };
    
  class Villager : public Character
  {
  public:
    int type();
  };
    
  class Hunter : public Character
  {
  public:
    int type();
  };
    
  class Witch : public Character
  {
    int poison,antidote;    //毒药解药的数量
  public:
    Witch();
    int type();
    void use_poison();      //使用毒药
    void use_antidote();    //使用解药
    bool have_poison();     //有毒药
    bool have_antidote();   //有解药
  };
    
  class Seer: public Character
  {
  public:
    int type();
  };
    
  class Guard : public Character
  {
    int per_guard = -1;
  public:
    void who_i_guard(int ch);   //守卫的人
    int type();
    int last();                 //上一晚守卫的人
  };
}

#endif
