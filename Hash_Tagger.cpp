#include <iostream>
#include <string.h>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
#define TAG_SIZE unsigned short


// Check if file exists
bool fileExists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}

// Used to store tags
class converter
{
	map<string, TAG_SIZE> tag;
	public:
	void init(string file);
	TAG_SIZE operator [] (string tag_name)
	{
		if(tag[tag_name] == 0)
			return -1;
		return tag[tag_name];
	}
};

// Read tags from file and assign values
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
		tag[s] = i++;
	}
	
}

// Read
void assign_vals(string directory, converter tags)
{
	string word;
	for(int i = 0;; ++i)
	{
		if(!fileExists((directory + to_string(i) + ".txt").c_str()) || !fileExists((directory + to_string(i) + ".key").c_str()))
		{
			cout<<"Reached "<<i<<".txt\n";
			break;
		}
		ifstream f((directory + to_string(i) + ".txt").c_str(), ios::in);
		ifstream vals((directory + to_string(i) + ".key").c_str(), ios::in);
		vector<TAG_SIZE> v;
		while(!vals.eof())
		{
			string s;
			getline(vals, s);
			if(tags[s] != -1)
				v.push_back(tags[s]);
		}
		bool skip = false;
		while(!f.eof())
		{
			char l;
			f.read((char *)&l, sizeof(l));
			if(l == '>')
				skip = false;
			else if(l == '<')
				skip = true;
			if(skip)
				continue;
			if(l == '\'')
				continue;
			if(!isalpha(l))
			{
				if(word.length() != 0)
				{
					map<TAG_SIZE, int> inp;
					if(fileExists((word + ".word").c_str()))
					{
						ifstream infile((word + ".word").c_str(), ios::in | ios::binary);
						TAG_SIZE t;
						int val;
						while(infile.read((char *) &t, sizeof(t)))
						{
							infile.read((char *) &val, sizeof(val));
							inp[t] = val;
						}
						infile.close();
					}
					for(vector<TAG_SIZE>::iterator it = v.begin(); it != v.end(); ++it)
						inp[*it]++;
					ofstream outfile((word + ".word").c_str(), ios::out | ios::binary);
					for(map<TAG_SIZE, int>::iterator it = inp.begin(); it != inp.end(); ++it)
					{
						outfile.write((char *) &it->first, sizeof(it->first));
						outfile.write((char *) &it->second, sizeof(it->second));
					}
					outfile.close();
				}
				word.clear();
			}
			else
			{
				word += l;
			}
		}
		f.close();
		vals.close();
	}
}
int main(int argv, char *argc[])
{
	if(argv < 3)
	{
		cout<<"Argument format: Vocabulary Test_Location\n";
		return 1;
	}
	converter tag;
	tag.init(argc[1]);
	assign_vals(argc[2], tag);
	return 0;
}

