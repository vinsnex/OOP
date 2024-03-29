#include "../../../include/LifeEntity/Roles/Roles.h"

Roles::Roles(Account* _user_account, string _name, int _LV, int nowHP, int nowMP, int Exp, string _Map_Now, RaceType _race, RoleType _role, Map_object _object) :
    user_account(_user_account),
    /*account(_account),*/
    name(_name),
    exp(Exp),
    Map_Now(_Map_Now),
    Role(_role),
    Map_object(_object),
    Race(_race),
    /*裝備*/
    Equipment(
        *_user_account,
        _name
    ),
    /*背包*/
    Back_Pack(
        *_user_account,
        GetPrivateProfileInt(_name.c_str(), "Money", INT_MAX, ("Data/Account/" + (_user_account->getAccount()) + "/Roles.ini").c_str())

    ),

    /*屬性*/
    LifeAttributes(
        _LV,
        nowHP == -1 ? sum_Attributes(_race, _role, "HP") * _LV : nowHP,
        nowMP == -1 ? sum_Attributes(_race, _role, "MP") * _LV : nowMP,
        sum_Attributes(_race, _role, "Attack"),
        sum_Attributes(_race, _role, "SP"),
        sum_Attributes(_race, _role, "DEF"),
        sum_Attributes(_race, _role, "CRT"),
        sum_Attributes(_race, _role, "Drop"),
        sum_Attributes(_race, _role, "HP"),
        sum_Attributes(_race, _role, "MP")
    ),
    /*Skill*/
    Skill_List(
        *_user_account,
        _race,
        _role
    )
{}

Roles::~Roles() {
    delete user_account;
}

//string Roles::getAccount() { return user_account.getAccount(); }

//string Roles::getName() { return name; }
int Roles::getExp() { return exp; }
int Roles::getUpExp() { return (int)(pow(getLV() - 1, 3) + 60); }
//int Roles::getDrop() { return drop; }

string Roles::setName(string _name) { return name = _name; }
int Roles::setExp(int _exp) { return exp = (_exp > getUpExp() ? getUpExp() : _exp); }
//int Roles::setDrop(int _drop) { return drop = drop > 100 ? 100 : drop; }
void Roles::setMap_Now(const string _map) { Map_Now = _map; }

int Roles::addExp(int _exp, bool percent) {
    int exp = ADD_FUN(getExp(), _exp, percent);
    //if (exp > getUpExp()) exp = getUpExp();
    return setExp(exp);
}

//int Roles::addDrop(int _drop) { return setDrop(getDrop() + _drop); }

bool Roles::isUpLv() { return getExp() >= getUpExp() ? true : false; }

bool Roles::UP_LV() {
    if (isUpLv()) {
        addLV();
        setHP(getMaxHP());
        setMP(getMaxMP());
        setExp(0);
        addSkill_Point();
        return true;
    }
    return false;
}

void Roles::Open_BackPack() {
    vector<Sub_Goods*>& goods = getBackPack_Goods();
    static int key, _row, i;
    _row = 0;
    while (true) {
        system("cls");
        show_State();
        cout << "==================================== 背包 ====================================" << endl;
        cout << "\t背包負重："; ShowWeith();
        Draw::SetColor(6); cout << "\t\t\t金幣：" << getMoney() << endl << endl; Draw::SetColor();

        for (i = _row * 10; i < goods.size() && i < (_row + 1) * 10; i++) {
            cout << left << setw(3) << to_string(i % 10) + "." << " ";
            goods[i]->show_info();
        }
        cout << endl;
        if (_row != 0) cout << "(Q) 返回上一頁 ";
        if (i < goods.size()) cout << "(W) 前往下一頁 ";
        cout << endl;
        cout << "(E) 卸下裝備 " << endl;

        key = _getch();

        switch (key)
        {
        case 27: case 'b': case 'B':
            return;
        case 'Q': case 'q':
            if (_row != 0) _row--;
            break;

        case 'w': case 'W':
            if (i < goods.size()) _row++;
            break;
        case 'e': case 'E':
            Remove_Equipment(getBackPack_Goods());
            break;
        default:
            if ((key = key - '0') >= 0 && key < 10 && key + _row * 10 < (int)goods.size()) {
                int sel = 0;
                system("cls");
                cout << "1. 使用物品\n2. 丟到子背包\n3. 丟棄物品\n";
                while (sel == 0) {
                    switch ((sel = _getch()))
                    {
                    case '1':
                        Back_Pack_User_Item(goods[(__int64)key + (__int64)_row * 10]);
                        break;
                    case '2':
                        if (Put_To_Sub_Back_Pack(goods[(__int64)key + (__int64)_row * 10])) {
                            RmGoods((__int64)key + (__int64)_row * 10);
                        }
                        break;
                    case '3':
                        static Sub_Goods * _good; _good = goods[(__int64)key + (__int64)_row * 10];

                        if (typeid(*_good) == typeid(Sub_Back_Pack)) {
                            ((Sub_Back_Pack*)_good)->Away_Sub_Back_Pack();
                        }
                        delete goods[(__int64)key + (__int64)_row * 10];
                        goods[(__int64)key + (__int64)_row * 10] = NULL;

                        break;
                    default:
                        sel = 0;
                        break;
                    }
                }

                for (vector<Sub_Goods*>::iterator it = goods.begin(); it != goods.end(); it++) {
                    if (*it == NULL) {
                        goods.erase(it);
                        if (goods.size() > 0)
                            it = goods.begin();
                        else break;
                    }
                }
            }
            break;
        }
    }
    Save_Roles();
}

void Roles::Back_Pack_User_Item(Sub_Goods*& _gds) {
    if (_gds->isUse()) {
        if (typeid(*_gds) == typeid(Sub_Back_Pack)) {
            static Sub_Back_Pack* _bk; _bk = (Sub_Back_Pack*)_gds;
            if (_bk->getNowWeith() != 0) {
                _bk->Open_Sub_Back_Pack(*this);
            }
            else
                if (getNowWeith() > _bk->getMaxWeith()) {
                    cout << "背包過重無法更換!" << endl;
                    system("pause");
                }
                else {
                    static Back_Pack_Szie swap;
                    swap = getBack_Pack_Szie_Type();
                    setBack_Pack_Szie_Type(_bk->getBack_Pack_Szie_Type());
                    _bk->setBack_Pack_Szie_Type(swap);
                }
        }
        else
            if (((Goods*)_gds)->Usable(get_RoleType(), getRaceType())) {
                if (getLV() >= ((Goods*)_gds)->getLV()) {
                    if (typeid(*_gds) == typeid(Equipment_Attributes)) {
                        _gds = Put_on((Equipment_Attributes*)_gds);
                    }
                    else if (typeid(*_gds) == typeid(Consumables)) {
                        ((Consumables*)_gds)->Use(*this);
                        if (_gds->getQuantity() == 0) {
                            delete _gds;
                            _gds = NULL;
                        }
                    }
                }
                else {
                    cout << "你的等級未到達 無法使用!" << endl;
                    system("pause");
                }
            }
            else {
                if (!((Goods*)_gds)->Usable_Role(get_RoleType())) cout << "你的職業無法使用!" << endl;
                if (!((Goods*)_gds)->Usable_Race(getRaceType())) cout << "你的種族無法使用!" << endl;
                cout << endl;
                system("pause");
            }
    }
}

int Roles::sum_Attributes(RaceType _race, RoleType _role, string _att) {
    return GetPrivateProfileInt(toString(_race).c_str(), _att.c_str(), INT_MAX, "Data/Attributes/Race.ini") + GetPrivateProfileInt(toString(_role).c_str(), _att.c_str(), INT_MAX, "Data/Attributes/Role.ini");
}

//string Roles::getRoleType_S() { return toString(role); }
//RoleType Roles::getRoleType() { return (role); }

string Roles::getMap_Now() { return Map_Now; }

int Roles::getMaxHP() {
    if (getHP() > LifeAttributes::getMaxHP() + Equipment::sumHP() + Skill_List::Sum_Skill_HP())
        setHP(LifeAttributes::getMaxHP() + Equipment::sumHP() + Skill_List::Sum_Skill_HP());
    return LifeAttributes::getMaxHP() + Equipment::sumHP() + Skill_List::Sum_Skill_HP();
}

int Roles::getMaxMP() {
    if (getMP() > LifeAttributes::getMaxMP() + Equipment::sumMP() + Skill_List::Sum_Skill_MP())
        setMP(LifeAttributes::getMaxMP() + Equipment::sumMP() + Skill_List::Sum_Skill_MP());
    return LifeAttributes::getMaxMP() + Equipment::sumMP() + Skill_List::Sum_Skill_MP();
}

int Roles::getAttack() { return LifeAttributes::getAttack() + Equipment::sumAttack() + Skill_List::Sum_Skill_Attack(); }

int Roles::getSP() { return LifeAttributes::getSP() + Equipment::sumSP() + Skill_List::Sum_Skill_SP(); }

int Roles::getDef() { return LifeAttributes::getDef() + Equipment::sumDef() + Skill_List::Sum_Skill_Def(); }

int Roles::getCRT() { return LifeAttributes::getCRT() + Equipment::sumCrt() + Skill_List::Sum_Skill_CRT(); }

int Roles::getDrop() { return LifeAttributes::getDrop() + Equipment::sumDrop() + Skill_List::Sum_Skill_Drop(); }

void Roles::Save_Roles_info() {
    string outfile = "Data/Account/" + (user_account->getAccount()) + "/Roles.ini";
    WritePrivateProfileString(name.c_str(), "LV", to_string(getLV()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "Race", (getRaceType_S()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "Role", getRoleType_S().c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "HP", to_string(getHP()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "MP", to_string(getMP()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "EXP", to_string(getExp()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "MapNow", getMap_Now().c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "ObjectX", to_string(get_Point_X()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "ObjectY", to_string(get_Point_Y()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "ObjectWidth", to_string(get_Width()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "ObjectHeigh", to_string(get_Heigh()).c_str(), outfile.c_str());
    WritePrivateProfileString(name.c_str(), "Money", to_string(getMoney()).c_str(), outfile.c_str());
}

//*********************************** 角色移動 ***********************************
/*物件移動 X*/
Map_object* Roles::set_Roles_Move_X(int _x, vector<Map_object*>& _object) {
    static Map_object* over;
    static int posX;
    over = NULL;
    posX = get_Point_X();
    Map_object::set_Point_X(_x);
    if ((over = Object_Overlapping(_object)) != NULL || get_Point_X() < 2 || get_Point_X() > MAP_WIDTH_DEF - get_Width()) {
        Map_object::set_Point_X(posX);
    }
    else {
        Map_object::set_Point_X(posX);
        clear_Draw_Object();
        Map_object::set_Point_X(_x);
    }
    return over;
}

/*物件移動 Y*/
Map_object* Roles::set_Roles_Move_Y(int _y, vector<Map_object*>& _object) {
    static Map_object* over;
    static int posY;
    over = NULL;
    posY = get_Point_Y();
    Map_object::set_Point_Y(_y);
    if ((over = Object_Overlapping(_object)) != NULL || get_Point_Y() < 1 || get_Point_Y() > MAP_HIGH_DEF - get_Heigh()) {
        Map_object::set_Point_Y(posY);
        //Y = posY;
    }
    else {
        Map_object::set_Point_Y(posY);
        clear_Draw_Object();
        Map_object::set_Point_Y(_y);
    }
    return over;
}

void Roles::Save_Roles() {
    Save_Roles_info();
    Save_BackPack();
    Save_Equipment();
    Save_Skill();
}

void Roles::show_State() {
    system("cls");
    cout << "\n\t\t\t角色狀態" << endl << endl;
    cout << setw(10) << "" << right << setw(10) << "角色名稱：" << " " << user_account->getRolesName() << endl;
    cout << setw(10) << "" << right << setw(10) << "種族：" << " " << getRaceType_S() << endl;
    cout << setw(10) << "" << right << setw(10) << "職業：" << " " << getRoleType_S() << endl;
    cout << setw(10) << "" << right << setw(10) << "等級：" << " " << getLV() << endl;
    cout << setw(10) << "" << right << setw(10) << "生命：" << " "; showHP(true, getMaxHP());
    cout << setw(10) << "" << right << setw(10) << "魔力：" << " "; showMP(true, getMaxMP());
    cout << setw(10) << "" << right << setw(10) << "經驗值：" << " "; show_EXP();
    cout << setw(10) << "" << right << setw(10) << "攻擊力：" << " " << getAttack() << endl;
    cout << setw(10) << "" << right << setw(10) << "速度：" << " " << getSP() << endl;
    cout << setw(10) << "" << right << setw(10) << "暴擊：" << " " << getCRT() << "%" << endl;
    cout << setw(10) << "" << right << setw(10) << "減傷：" << " " << getDef() << "%" << endl;
    cout << setw(10) << "" << right << setw(10) << "掉寶率：" << " " << getDrop() << "%" << endl;
    show_Equipment();
}

void Roles::show_EXP(bool show, bool LF) {
    Draw::SetColor(2);
    cout << SHOW_EXP_FRAME_BEGIN;
    for (int i = 0; i < SHOW_MAX_EXP; i++)
        cout << SHOW_EXP_FIGURE(i < (getExp() * (SHOW_MAX_EXP / (double)(getUpExp()))));
    cout << SHOW_MP_FRAME_END;
    Draw::SetColor();
    if (show) cout << " " << left << setw(4) << getExp() << "/" << left << setw(4) << getUpExp();
    if (LF) cout << endl;
    //return this;
}

//Map_object* Roles::Object_overlapping(Map_object& obj, vector<Map_object*>& _object) {
//    for (vector<Map_object*>::iterator it = _object.begin(); it != _object.end(); it++)
//        if (obj == (**it))
//            return *it;
//    return NULL;
//}
//**********************************************************************************************