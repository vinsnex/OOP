#include "../../include/Account/RolesList.h"

RolesList::RolesList() :user_account(), roles(NULL) {}

RolesList::~RolesList() { if (roles != NULL) delete roles; }

void RolesList::RolesListMenu() {
    bool isEnd = false;
    if (user_account.IsSignIn() == false) user_account.AccountMenu();
    while (!isEnd) {
        system("cls");
        cout << "1. 選擇角色" << endl;
        cout << "2. 新增角色" << endl;
        cout << "3. 刪除角色" << endl;
        cout << "4. 返回登入畫面" << endl << endl;
        cout << "5. 刪除此帳號" << endl;
        switch (_getch())
        {
        case '1':
            isEnd = Selete_Roles();
            break;
        case '2':
            addRoles();
            break;
        case '3':
            DeleteRoles();
            break;
        case '4':
            if (roles != NULL) {
                delete roles;
                roles = NULL;
            }
            user_account.Sign_out();

            isEnd = true;
            break;
        case '5':
            isEnd = user_account.rmAccount();
            if (isEnd) {
                if (roles != NULL) {
                    delete roles;
                    roles = NULL;
                }
                user_account.Sign_out();
            }
            break;
        default:
            break;
        }
    }
}

void RolesList::DeleteRoles() {
    bool isExit = false;
    vector<Roles*> roleslist;
    int getKey;
    while (!isExit) {
        system("cls");
        FindRoles(roleslist);
        if (roleslist.size() == 0) {
            cout << "此帳號沒有任何角色請先新增角色!" << endl;
            system("pause");
            isExit = true;
        }
        else {
            cout << "刪除角色!(按 Ese 離開)" << endl;

            show_Total_RolesList(roleslist, 2);
            //for (unsigned int i = 0; i < roleslist.size(); i++) {
            //    cout << i+1 <<". " << roleslist[i]->getName() << endl;
            //}

            getKey = _getch();
            if (getKey == 27)
                isExit = true;
            else if ((unsigned int)(getKey = getKey - 1 - '0') < roleslist.size()) {
                WritePrivateProfileString(roleslist[getKey]->user_account->getRolesName().c_str(), NULL, NULL, ("Data/Account/" + user_account.account + "/Roles.ini").c_str());
                static string dir;
                dir = "Data\\Account\\" + user_account.account + "\\" + roleslist[getKey]->user_account->getRolesName();
                if (Tool::CheckFolderExist(dir)) {
                    system(("rmdir /s /q " + dir).c_str());
                    system("cls");
                }
                cout << "刪除成功!" << endl;
                system("pause");
            }
        }
        clearRolesList(roleslist);
    }
}

void RolesList::Clear_Now_Roles() {
    delete roles;
    roles = NULL;
}

void RolesList::Save_Roles() {
}

bool RolesList::addRoles() {
    string name;
    RaceType race_type = RaceType::Human;
    RoleType role_type = RoleType::Magician;
    vector<string> namelist;
    bool isEnd = false;
    int getKey;

    while (1) {
        system("cls");
        Tool::ReadIpAppName("Data/Account/" + user_account.account + "/Roles.ini", namelist);
        if (namelist.size() > 2) {
            isEnd = true;
            cout << "角色以到達上限!" << endl;
            system("pause");
            break;
        }
        cout << "請輸入角色名稱(exit 離開):";
        cin >> name;
        if (name == "exit" || name == "Exit") return false;

        if (find(namelist.begin(), namelist.end(), name) == namelist.end())
            break;
        cout << name << " 已被使用過!" << endl;
        namelist.clear();
        system("pause");
    }
    namelist.clear();
    system("cls");

    if (!isEnd) {
        isEnd = false;
        vector<string> TypeList;
        Tool::ReadIpAppName("Data/Attributes/Race.ini", TypeList);
        show_Total_RaceInfo(TypeList, "Data/Attributes/Race.ini", 0);

        while (!isEnd) {
            getKey = _getch();
            if (getKey == 27) {
                break;
            }if ((unsigned int)(getKey = getKey - '0' - 1) < TypeList.size()) {
                race_type = toRace_Type(TypeList[getKey]);
                isEnd = true;
            }
        }
        TypeList.clear();

        if (isEnd) {
            isEnd = false;
            system("cls");
            Tool::ReadIpAppName("Data/Attributes/Role.ini", TypeList);
            show_Total_RaceInfo(TypeList, "Data/Attributes/Role.ini", 0);
            while (!isEnd) {
                getKey = _getch();
                if (getKey == 27) {
                    break;
                }if ((unsigned int)(getKey = getKey - '0' - 1) < TypeList.size()) {
                    role_type = toRole_Type(TypeList[getKey]);
                    isEnd = true;
                }
            }
            TypeList.clear();

            if (isEnd) {
                system("cls");
                cout << setw(10) << "角色名稱：" << name << endl;
                cout << setw(10) << "種族：" << toString(race_type) << endl;
                cout << setw(10) << "職業：" << toString(role_type) << endl;
                cout << "確定新增(y) 取消(N)" << endl;
                while (1) {
                    if ((getKey = _getch()) == 'y' || getKey == 'Y' || getKey == 'n' || getKey == 'N')
                        break;
                }
                if (getKey == 'y' || getKey == 'Y') {
                    Tool::mkdir("Data/Account/" + user_account.account);
                    string outfile = "Data/Account/" + user_account.account + "/Roles.ini";
                    WritePrivateProfileString(name.c_str(), "LV", "1", outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "Race", toString(race_type).c_str(), outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "Role", toString(role_type).c_str(), outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "HP", to_string(Roles::sum_Attributes(race_type, role_type, "HP")).c_str(), outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "MP", to_string(Roles::sum_Attributes(race_type, role_type, "MP")).c_str(), outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "EXP", "0", outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "MapNow", "市集", outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "ObjectX", "50", outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "ObjectY", "22", outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "ObjectWidth", ROLES_OBJECT_WIDTH, outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "ObjectHeigh", ROLES_OBJECT_HEIGHT, outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "Money", "100", outfile.c_str());
                    WritePrivateProfileString(name.c_str(), "Skill_Point", "1", outfile.c_str());

                    cout << "新增角色成功!" << endl;
                    system("pause");
                    return true;
                }
            }
        }
    }
    return false;
}

void RolesList::FindRoles(vector<Roles*>& out) {
    Roles* roles;
    string inifile = "Data/Account/" + user_account.account + "/Roles.ini";
    vector<string> roleslist;
    Tool::ReadIpAppName(inifile, roleslist);
    CString read_ini;
    RaceType race;
    RoleType role;
    for (vector<string>::iterator it = roleslist.begin(); it != roleslist.end(); it++) {
        GetPrivateProfileString(it->c_str(), "Race", "NULL", read_ini.GetBuffer(255), 255, inifile.c_str());
        read_ini.ReleaseBuffer();
        race = toRace_Type(string(read_ini));
        GetPrivateProfileString(it->c_str(), "Role", "NULL", read_ini.GetBuffer(255), 255, inifile.c_str());
        read_ini.ReleaseBuffer();
        role = toRole_Type(string(read_ini));
        user_account.Roles_Name = *it;
        roles = new Roles(
            new Account(user_account),
            *it,
            GetPrivateProfileInt((*it).c_str(), "LV", INT_MAX, inifile.c_str()),
            GetPrivateProfileInt((*it).c_str(), "HP", INT_MAX, inifile.c_str()),
            GetPrivateProfileInt((*it).c_str(), "MP", INT_MAX, inifile.c_str()),
            GetPrivateProfileInt((*it).c_str(), "EXP", INT_MAX, inifile.c_str()),
            Tool::ReadStringIni(*it, "MapNow", "NULL", inifile),
            race,
            role,
            Map_object("data/Image/object.txt",
                "Warrior",
                objectType::Warrior,
                GetPrivateProfileInt((*it).c_str(), "ObjectX", INT_MAX, inifile.c_str()),
                GetPrivateProfileInt((*it).c_str(), "ObjectY", INT_MAX, inifile.c_str()),
                GetPrivateProfileInt((*it).c_str(), "ObjectWidth", INT_MAX, inifile.c_str()),
                GetPrivateProfileInt((*it).c_str(), "ObjectHeigh", INT_MAX, inifile.c_str())
            )
        );
        out.push_back(roles);
    }
}

bool RolesList::Selete_Roles() {
    vector<Roles*> roleslist;
    bool isExit = false;
    unsigned int getKey;
    while (roles == NULL && !isExit) {
        FindRoles(roleslist);
        system("cls");
        cout << "共有 " << roleslist.size() << "/3 隻角色!" << endl;
        if (roleslist.size() == 0) {
            cout << "此帳號沒有任何角色前往新增角色!" << endl;
            system("pause");

            return addRoles();
        }

        cout << "請選擇角色:(Esc離開)" << endl;
        show_Total_RolesList(roleslist, 3);

        while (!isExit) {
            getKey = _getch();
            if (getKey == 27)
                isExit = true;
            else if ((getKey = getKey - '0' - 1) < roleslist.size()) {
                roles = roleslist[getKey];

                //user_account.user_account.getRolesName());
                isExit = true;
            }
        }

        clearRolesList(roleslist);
    }
    return roles == NULL ? false : true;
}

void RolesList::clearRolesList(vector<Roles*>& _list) {
    for (vector<Roles*>::iterator it = _list.begin(); it != _list.end(); it++)
        if (*it != roles)
            delete* it;
    _list.clear();
}

Roles* RolesList::getRoles() { return roles; }

Account* RolesList::getAccount()
{
    return &user_account;
}

void RolesList::show_Total_RolesList(vector<Roles*>& roleslist, unsigned int _row) {
    static unsigned int row;
    for (unsigned int i = 0; i < roleslist.size(); i++) {
        row = _row;
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << "￣￣￣￣￣￣￣";
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << (i + 1);
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << "∼∼∼∼∼∼∼";
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << roleslist[i]->user_account->getRolesName();
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << setw(8) << "等級：" << roleslist[i]->getLV();
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << setw(8) << "種族：" << roleslist[i]->getRaceType_S();
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << setw(8) << "職業：" << roleslist[i]->getRoleType_S();
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << setw(8) << "攻擊力：" << roleslist[i]->getAttack();
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << setw(8) << "HP：" << roleslist[i]->getMaxHP();
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << setw(8) << "MP：" << roleslist[i]->getMaxMP();
        Draw::setXY(SELROLESP_COL(i), row++); cout << "｜" << "ˍˍˍˍˍˍˍ";
    }
    for (int i = 0; i < 11; i++) { Draw::setXY(SELROLESP_COL(roleslist.size()), _row + i); cout << "｜"; }
    Draw::setXY(0, row++);
}

void RolesList::show_Total_RoleInfo(const vector<string>& rolesList, const string _file, const unsigned int _row) {
    static unsigned int row;
    for (unsigned int i = 0; i < rolesList.size(); i++) {
        row = _row;
        Draw::setXY(SHOWRACEINFO_COL(i), row++); for (int count = 0; count < SHOWRACEINFO_COUNT; count++) cout << "￣";
        //Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜";
        row++;
        Draw::setXY(SHOWRACEINFO_COL(i), row++); for (int count = 0; count < SHOWRACEINFO_COUNT; count++) cout << "∼";
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(4) << (i + 1) << ". " << left << setw(10) << rolesList.at(i);
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜"; for (int count = 0; count < SHOWRACEINFO_COUNT - 1; count++) cout << "∵";
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "攻擊力：" << left << setw(6) << GetPrivateProfileInt(rolesList[i].c_str(), "Attack", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "HP：" << left << setw(6) << GetPrivateProfileInt(rolesList[i].c_str(), "HP", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "MP：" << left << setw(6) << GetPrivateProfileInt(rolesList[i].c_str(), "MP", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "速度：" << left << setw(6) << GetPrivateProfileInt(rolesList[i].c_str(), "SP", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "爆擊：" << left << setw(6) << GetPrivateProfileInt(rolesList[i].c_str(), "CRT", INT_MAX, _file.c_str()) << "%";
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "減傷：" << left << setw(6) << GetPrivateProfileInt(rolesList[i].c_str(), "DEF", INT_MAX, _file.c_str()) << "%";
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "調寶率：" << left << setw(6) << GetPrivateProfileInt(rolesList[i].c_str(), "Drop", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); for (int count = 0; count < SHOWRACEINFO_COUNT; count++) cout << "ˍ";
    }
    for (int i = 0; i < 13; i++) {
        Draw::setXY(0, _row + i); cout << "｜";
        Draw::setXY(SHOWRACEINFO_COL(rolesList.size()), _row + i); cout << "｜";
    }

    Draw::setXY(SHOWRACEINFO_COUNT * 2 * (int)rolesList.size() / 2 - 2, _row + 1); cout << "職業";
    Draw::setXY(0, row);
}

void RolesList::show_Total_RaceInfo(const vector<string>& raceList, const string _file, const unsigned int _row) {
    static unsigned int row;
    for (unsigned int i = 0; i < raceList.size(); i++) {
        row = _row;
        Draw::setXY(SHOWRACEINFO_COL(i), row++); for (int count = 0; count < SHOWRACEINFO_COUNT; count++) cout << "￣";
        //Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜";
        row++;
        Draw::setXY(SHOWRACEINFO_COL(i), row++); for (int count = 0; count < SHOWRACEINFO_COUNT; count++) cout << "∼";
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(4) << (i + 1) << ". " << left << setw(10) << raceList.at(i);
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜"; for (int count = 0; count < SHOWRACEINFO_COUNT - 1; count++) cout << "∵";
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "攻擊力：" << left << setw(6) << GetPrivateProfileInt(raceList[i].c_str(), "Attack", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "HP：" << GetPrivateProfileInt(raceList[i].c_str(), "HP", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "MP：" << GetPrivateProfileInt(raceList[i].c_str(), "MP", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "速度：" << GetPrivateProfileInt(raceList[i].c_str(), "SP", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "爆擊：" << GetPrivateProfileInt(raceList[i].c_str(), "CRT", INT_MAX, _file.c_str()) << "%";
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "減傷：" << GetPrivateProfileInt(raceList[i].c_str(), "DEF", INT_MAX, _file.c_str()) << "%";
        Draw::setXY(SHOWRACEINFO_COL(i), row++); cout << "｜" << right << setw(8) << "調寶率：" << GetPrivateProfileInt(raceList[i].c_str(), "Drop", INT_MAX, _file.c_str());
        Draw::setXY(SHOWRACEINFO_COL(i), row++); for (int count = 0; count < SHOWRACEINFO_COUNT; count++) cout << "ˍ";
    }
    for (int i = 0; i < 13; i++) {
        Draw::setXY(0, _row + i); cout << "｜";
        Draw::setXY(SHOWRACEINFO_COL(raceList.size()), _row + i); cout << "｜";
    }

    Draw::setXY(SHOWRACEINFO_COUNT * 2 * (int)raceList.size() / 2 - 2, _row + 1); cout << "種族";
    Draw::setXY(0, row);
}