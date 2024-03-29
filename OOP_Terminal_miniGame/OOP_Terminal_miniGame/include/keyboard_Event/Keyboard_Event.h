#ifndef KEYBOARD_EVENT_H
#define KEYBOARD_EVENT_H

#include<string>
#include "../Map/Game_Map.h"
#include "../Account/RolesList.h"
using namespace std;

class Keyboard_Event {
private:
    map<string, Game_Map*>* maplist;
    RolesList* roleslist;
    int Buff_Key;
public:
    Keyboard_Event(map<string, Game_Map*>* _map, RolesList* _roleslist);
    void Run();

private:
    bool Overlapping_Object(Map_object*);
    void Read_Key(bool& _isDrawMap);
    void Show_Map_Pos();
    void Esc_Table();
    void Back_To_Selete_Roles();
    void Back_To_Selete_Account();

private:
    void Fighting_Fun(Monster*);
};

#endif