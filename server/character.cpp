#include "character.h"
using namespace Werewolf;

void Character :: set_police()
{
    police = 1;
}

void Character :: set_dead()
{
    dead = 1;
}

void Character :: guardfunc()
{
    _guard = 1;
}

void Character :: set_alive()
{
    dead = 0;
}

bool Character :: is_police()
{
    return police;
}

bool Character :: is_dead()
{
    return dead;
}

bool Character :: is_guarded()
{
    return _guard;
}

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

Witch :: Witch() : poison(1), antidote(1) {}    //一瓶毒药，一瓶解药

void Witch :: use_poison()  //使用毒药
{
    poison = 0;
}

void Witch :: use_antidote()    //使用解药
{
    antidote = 0;
}

bool Witch :: have_poison()
{
    return poison;
}

bool Witch :: have_antidote()
{
    return antidote;
}

void Guard :: who_i_guard(int ch)
{
    per_guard = ch;
}
int Guard :: last()   //上一晚守卫的人的编号
{
    return per_guard;
}
