#ifndef TXTFUNCSHPP
#define TXTFUNCSHPP

#include <common.hpp>

class GameObject;

#define txtFuncType1 function<void(GameObject*)>
#define txtFuncType2(T) function<txtFuncType1(T&)>

extern map<string,txtFuncType2(ifstream)> txtFuncsF;
extern map<string,txtFuncType2(istringstream)> txtFuncsS;

// template<class T> txtFuncType1 AddParticle(T& in);
template<class T> txtFuncType1 AddSprite(T& in);
template<class T> txtFuncType1 AddVar(T& in);
template<class T> txtFuncType1 ChangeVar(T& in);
template<class T> txtFuncType1 Damage(T& in);
template<class T> txtFuncType1 DamageArea(T& in);
template<class T> txtFuncType1 DamageAreaFixed(T& in);
template<class T> txtFuncType1 FireProjectile(T& in);
template<class T> txtFuncType1 Remove(T& in);

#endif//TXTFUNCSHPP
