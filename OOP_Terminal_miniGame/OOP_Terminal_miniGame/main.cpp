#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <map>
#include "include/Map/Draw.h"
#include "include/Map/Game_Map.h"
#include "include/Account/Account.h"
#include "include/Account/RolesList.h"
#include "include/keyboard_Event/Keyboard_Event.h"

using namespace std;

void Game_start();
void Game_initial();
void Game_Exit();

map<string, Game_Map*> maplist;
RolesList roleslist;
Keyboard_Event keyboard_event(&maplist, &roleslist);

int main() {
    Game_initial();
    Game_start();
    Game_Exit();
    return 0;
}

void Game_initial() {
    system("mode con cols=200 lines=50");
    srand((unsigned int)time(NULL));
    Game_Map::Read_Map(maplist);
}

void Game_start() {
    keyboard_event.Run();
}

void Game_Exit() {
}