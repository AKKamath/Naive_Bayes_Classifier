#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argv, char* argc[])
{
	if(argv != 2 && argv != 3)
	{
		cout<<"Invalid arguments. Argument passed should be file to be parsed";
		return 1;
	}
	string out_file = "out.txt";
	if(argv == 3)
	{
		out_file = argc[2];
	}
	ifstream f(argc[1], ios::in);
	ofstream f2(out_file.c_str(), ios::out);
	while(!f.eof())
	{
		char b;
		f.read((char*)&b, sizeof(b));
		if(b == '"')
			continue;
		f2<<b;
	}
	return 0;
}
