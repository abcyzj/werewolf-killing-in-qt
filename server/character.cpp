/*******************************************
 名称：character.cpp
 作者:王琛 计65 2016011360
 时间：2017/05/15
 内容：角色类中各种函数的实现
 版权：自行完成的程序，没有使用其余的源代码
 *******************************************/

#include "character.h"
using namespace Werewolf;

void Character :: set_police()  //将该角色设置为警长
{
  police = 1;
}

void Character :: set_dead()    //将该角色设置为死亡
{
  dead = 1;
}

void Character :: guardfunc()   //将该角色置于被守卫状态
{
  _guard = 1;
}

void Character :: set_alive()   //将该角色救活
{
  dead = 0;
}

bool Character :: is_police()   //判断该角色是否为警长
{
  return police;
}

bool Character :: is_dead()     //判断该角色是否已经死亡
{
  return dead;
}

bool Character :: is_guarded()  //判断该角色是否被守卫
{
  return _guard;
}

//以下是返回各个角色的类型
int Wolf :: type()
{
  return wolf;
}

int Villager :: type()
{
  return villager;
}

int Hunter :: type()
{
  return hunter;
}

int Witch :: type()
{
  return witch;
}

int Seer :: type()
{
  return seer;
}

int Guard :: type()
{
  return guard;
}
//到此为止

Witch :: Witch() : poison(1), antidote(1) {}//女巫开始一瓶毒药，一瓶解药

void Witch :: use_poison()  //使用毒药
{
  poison = 0;
}

void Witch :: use_antidote() //使用解药
{
  antidote = 0;
}

bool Witch :: have_poison()  //判断是否剩下毒药
{
  return poison;
}

bool Witch :: have_antidote()//判断是否剩下解药
{
  return antidote;
}

void Guard :: who_i_guard(int ch) //被守卫的人编号，守卫输入时传入
{
  per_guard = ch;
}
int Guard :: last()   //上一晚守卫的人的编号
{
  return per_guard;
}
