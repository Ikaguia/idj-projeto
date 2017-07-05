#ifndef TXTFUNCSHPP
#define TXTFUNCSHPP

#include <common.hpp>

class GameObject;

#define txtFuncType1 function<void(GameObject*)>
#define txtFuncType2(T) function<txtFuncType1(T&)>

extern map<string,txtFuncType2(ifstream)> txtFuncsF;
extern map<string,txtFuncType2(istringstream)> txtFuncsS;

void CallFunc(const string func,const string str,GameObject* go);

#endif//TXTFUNCSHPP
