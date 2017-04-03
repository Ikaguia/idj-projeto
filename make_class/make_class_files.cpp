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
	for(auto &i:s){
		i=toUpperChar(i);
	}
	return s;
}

void makeHPP(string s){
	ofstream file;
	file.open("include/"+s+".hpp");
	if(!file.is_open())return;

	string className = toUpperChar(s[0]) + s.substr(1);
	string defName = toUpper(s) + "HPP";

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

int main(int argc,char **argv){
	if(argc<=1)return 0;
	string s=argv[1];
	if(arc==2){
		makeCPP(s);
		makeHPP(s);
	}
	if(argc==3){
		makeHPP(s);
	}
	if(argc==4){
		makeHPP(s);
	}
}
0
