#include <string>
#include <iostream>
#include <map>
#include <fstream>
using namespace std;
bool fileExists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

class converter
{
	map<string, unsigned short> tag;
	map<unsigned short, string> tag2;
	public:
	void init(string file);
	short operator [] (string tag_name)
	{
		if(tag[tag_name] == 0)
			return -1;
		return tag[tag_name];
	}
	string operator [](unsigned short val)
	{
		return tag2[val];
	}
};

void converter::init(string file)
{
	if(!fileExists(file.c_str()))
	{
		cout<<"Error, "<<file<<" does not exist!\n";
		return;
	}
	ifstream f(file.c_str(), ios::in);
	int i = 0;
	while(!f.eof())
	{
		string s;
		getline(f, s);
		tag2[i] = s;
		tag[s] = i++;
	}
	
}

int main(int argv, char *argc[])
{
	if(argv < 2)
	{
		cout<<"Argument format: Vocabulary\n";
		return 1;
	}
	converter tag;
	tag.init(argc[1]);
	bool opt;
	do
	{
		cout<<"1: String->Int 2: Int->String\n";
		int opt;
		cin>>opt;
		switch(opt)
		{
			case 1:
			{string s;
			cin>>s;
			
			cout<<tag[s]<<" ";}
			break;
			case 2:
			{
				unsigned short s;
				cin>>s;
				cout<<tag[s]<<" ";
			}			
		}
		cout<<"\nContinue?\n";
		cin>>opt;
	}while(opt);
	return 0;
}
