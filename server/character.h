#ifndef CHARACTER_H
#define CHARACTER_H

//村民 狼人 女巫 守卫 猎人 预言家
namespace Werewolf
{
    enum ch_type {wolf = 1, villager, hunter, witch, seer, guard};
    
    class Character
    {
        bool police = 0;
        bool dead = 0;
        bool _guard = 0;
    public:
        virtual int type() = 0; //返回一个int值代表角色类型
        bool is_dead();     //是否死亡
        bool is_police();	//是否为警长
        bool is_guarded();  //处于被守护状态
        void guardfunc();   //守卫
        void set_alive();   //复活（女巫解药）
        void set_dead();    //被杀死
        void set_police();  //设为警长
    };
    
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
        int poison,antidote;
    public:
        Witch();
        int type();
        void use_poison();
        void use_antidote();
        bool have_poison();
        bool have_antidote();
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
        void who_i_guard(int ch);
        int type();
        int last();  //  上一晚守卫的人
    };
}

#endif
