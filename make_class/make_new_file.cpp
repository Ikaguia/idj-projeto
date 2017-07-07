#include <bits/stdc++.h>
using namespace std;

char toUpperChar(char c){
	if(c>='a' && c<='z'){
		c-='a';
		c+='A';
	}
	return c;
}

string toUpper(string s){
	for(char &i:s)i=toUpperChar(i);
	return s;
}

char toLowerChar(char c){
	if(c>='A' && c<='Z'){
		c-='A';
		c+='a';
	}
	return c;
}

string toLower(string s){
	for(char &i:s)i=toLowerChar(i);
	return s;
}

void makeHPP(string s){
	ofstream file;
	file.open("include/"+s+".hpp");
	if(!file.is_open())return;

	string className = toUpperChar(s[0]) + s.substr(1);
	string defName = (string)"COMP" + toUpper(s) + "HPP";

	file << "#ifndef " << defName << endl;
	file << "#define " << defName << endl;
	file << endl;
	file << "#include <common.hpp>" << endl;
	file << endl;
	file << "class " << className << "{" << endl;
	file << "	//private members" << endl;
	file << "public:" << endl;
	file << "	//public members" << endl;
	file << endl;
	file << "	" << className << "();" << endl;
	file << "	~" << className << "();" << endl;
	file << "};" << endl;
	file << endl;
	file << "#endif//" << defName << endl;

	file.close();
}

void makeCPP(string s){
	ofstream file;
	file.open("src/"+s+".cpp");
	if(!file.is_open())return;

	string className = toUpperChar(s[0]) + s.substr(1);

	file << "#include <"+s+".hpp>" << endl;
	file << "//#include <game.hpp>" << endl;
	file << "//#include <camera.hpp>" << endl;
	file << "//#include <inputManager.hpp>" << endl;
	file << endl;
	file << className << "::" << className << "(){}" << endl;
	file << className << "::~" << className << "(){}" << endl;

	file.close();
}

void makeCOMPONENT_HPP(string s){
	ofstream file;
	file.open("include/component"+s+".hpp");
	if(!file.is_open())return;

	string className = (string)"Comp" + s;
	string defName = "COMP" + toUpper(s) + "HPP";

	file << "#ifndef " << defName << endl;
	file << "#define " << defName << endl;
	file << endl;
	file << "#include <common.hpp>" << endl;
	file << "#include <component.hpp>" << endl;
	file << endl;
	file << "#define COMP" << toUpper(s) << "(x)  ((" << className << "*)x. components[Component::type::t_" << toLower(s) << "])" << endl;
	file << "#define COMP" << toUpper(s) << "p(x) ((" << className << "*)x->components[Component::type::t_" << toLower(s) << "])" << endl;
	file << endl;
	file << "class " << className << " : public Component{" << endl;
	file << "	//private members" << endl;
	file << "public:" << endl;
	file << "	//public members" << endl;
	file << endl;
	file << "	" << className << "();" << endl;
	file << "	~" << className << "();" << endl;
	file << "	void Update(float time);" << endl;
	file << "	void Render();" << endl;
	file << "	void Own(GameObject* go);" << endl;
	file << "	Component::type GetType()const;" << endl;
	file << "};" << endl;
	file << endl;
	file << "#endif//" << defName << endl;

	file.close();
}

void makeCOMPONENT_CPP(string s){
	ofstream file;
	file.open("src/component"+s+".cpp");
	if(!file.is_open())return;

	string className = (string)"Comp" + s;

	file << "#include <component"+s+".hpp>" << endl;
	file << "//#include <compLib.hpp>" << endl;
	file << "//#include <game.hpp>" << endl;
	file << "//#include <camera.hpp>" << endl;
	file << "//#include <inputManager.hpp>" << endl;
	file << endl;
	file << endl;
	file << className << "::" << className << "(){}" << endl;
	file << className << "::~" << className << "(){}" << endl;
	file << endl << endl;
	file << "void " << className << "::Update(float time){" << endl;
	file << "	UNUSED(time);" << endl;
	file << "}" << endl;
	file << endl << endl;
	file << "void " << className << "::Render(){}" << endl;
	file << endl << endl;
	file << "void " << className << "::Own(GameObject* go){" << endl;
	file << "	entity=go->uid;" << endl;
	file << "}" << endl;
	file << endl << endl;
	file << "Component::type " << className << "::GetType() const{" << endl;
	file << "	return Component::type::t_" << toLower(s) << ";" << endl;
	file << "}" << endl;
	file << endl << endl;

	file.close();
}

int main(int argc,char **argv){
	if(argc<=1)return 0;
	string s=argv[1];
	if(argc==2){
		makeCPP(s);
		makeHPP(s);
	}
	if(argc==3){
		makeHPP(s);
	}
	if(argc==4){
		makeHPP(s);
	}
	if(argc==5){
		makeCOMPONENT_CPP(s);
		makeCOMPONENT_HPP(s);
	}
}
