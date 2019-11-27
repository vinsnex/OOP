#include "../../include/LifeEntity/LifeAttributes.h"

LifeAttributes::LifeAttributes(int _LV, int _HP, int _MP, int _attack, int _sp, int _def, int _CRT,int _maxHP, int _maxMP):
    Attributes( _HP, _MP, _attack, _sp, _def, _CRT),
    LV(_LV),
    maxHP(maxHP == -1 ? _HP : maxHP),
    maxMP(maxMP == -1 ? _MP : maxMP)
{}



int LifeAttributes::getMaxHP() { return maxHP; }
int LifeAttributes::getMaxMP() { return maxMP; }
int LifeAttributes::getLV()    { return LV; }


int LifeAttributes::setHP(int _num) { return Attributes::setHP(_num > getMaxHP() ? getMaxHP() : _num); }
int LifeAttributes::setMP(int _num) { return Attributes::setMP(_num > getMaxMP() ? getMaxMP() : _num); }
int LifeAttributes::setMaxHP(int _num) { return maxHP = _num; }
int LifeAttributes::setMaxMP(int _num) { return maxMP = _num; }
int LifeAttributes::setLV(int _lv) { return LV = _lv; }

int LifeAttributes::addHP(int _hp, bool percent) { return setHP(ADD_FUN(getHP(), _hp, percent)); }
int LifeAttributes::addMP(int _mp, bool percent) { return setMP(ADD_FUN(getMP(), _mp, percent)); }
int LifeAttributes::addMaxHP(int _maxhp, bool percent) {
    addHP(_maxhp);
    return setHP(ADD_FUN(getHP(), _maxhp, percent));
}
int LifeAttributes::addMaxMP(int _maxmp, bool percent) {
    addMP(_maxmp);
    return setHP(ADD_FUN(getHP(), _maxmp, percent));
}

int LifeAttributes::addLV(int _add) { return setLV(getLV() + _add); }